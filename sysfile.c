#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "file.h"
#include "fcntl.h"

// Fetch the nth word-sized system call argument as a file descriptor
// and return both the descriptor and the corresponding struct file.
static int
argfd(int fd, int *pfd, struct file **pf)
{
  //int fd;
  struct file *f;
  if(fd < 0)
    return -1;
  if(fd < 0 || fd >= NOFILE || (f=proc->ofile[fd]) == 0)
    return -1;
  if(pfd)
    *pfd = fd;
  if(pf)
    *pf = f;
  return 0;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
static int
fdalloc(struct file *f)
{
  int fd;

  for(fd = 0; fd < NOFILE; fd++){
    if(proc->ofile[fd] == 0){
      proc->ofile[fd] = f;
      return fd;
    }
  }
  return -1;
}

int
sys_dup(int n)
{
  struct file *f;
  int fd;
  
  if(argfd(n, 0, &f) < 0)
    return -1;
  if((fd=fdalloc(f)) < 0)
    return -1;
  filedup(f);
  return fd;
}

int
sys_read(int m, char *p, int n)
{
  struct file *f;
  //int n;
  //char *p;

  if(argfd(m, 0, &f) < 0 || n < 0 || p < 0)
    return -1;
  return fileread(f, p, n);
}

int
sys_write(int m, char *p, int n)
{
  struct file *f;
  //int n;
  //char *p;
  if(argfd(m, 0, &f) < 0 || n < 0 || p < 0)
//  if(m < 0 || n < 0 || p < 0)
    return -1;
  return filewrite(f, p, n);
}

int
sys_close(int n)
{
  int fd;
  struct file *f;
  
  if(argfd(n, &fd, &f) < 0)
    return -1;
  proc->ofile[fd] = 0;
  fileclose(f);
  return 0;
}

int
sys_fstat(int m, struct stat *st)
{
  struct file *f;
  //struct stat *st;
  
  if(argfd(m, 0, &f) < 0 || st < 0)
    return -1;
  return filestat(f, st);
}

// Create the path new as a link to the same inode as old.
int
sys_link(char *old, char *new)
{
  char name[DIRSIZ];
  struct inode *dp, *ip;

  if(old < 0 || new < 0)
    return -1;
  if((ip = namei(old)) == 0)
    return -1;
  ilock(ip);
  if(ip->type == T_DIR){
    iunlockput(ip);
    return -1;
  }
  ip->nlink++;
  iupdate(ip);
  iunlock(ip);

  if((dp = nameiparent(new, name)) == 0)
    goto bad;
  ilock(dp);
  if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
    iunlockput(dp);
    goto bad;
  }
  iunlockput(dp);
  iput(ip);
  return 0;

bad:
  ilock(ip);
  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);
  return -1;
}

// Is the directory dp empty except for "." and ".." ?
static int
isdirempty(struct inode *dp)
{
  int off;
  struct dirent de;

  for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
    if(readi(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
      panic("isdirempty: readi");
    if(de.inum != 0)
      return 0;
  }
  return 1;
}

int
sys_unlink(char *path)
{
  struct inode *ip, *dp;
  struct dirent de;
  char name[DIRSIZ];
  uint off;

  if(path < 0)
    return -1;
  if((dp = nameiparent(path, name)) == 0)
    return -1;
  ilock(dp);

  // Cannot unlink "." or "..".
  if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0){
    iunlockput(dp);
    return -1;
  }

  if((ip = dirlookup(dp, name, &off)) == 0){
    iunlockput(dp);
    return -1;
  }
  ilock(ip);

  if(ip->nlink < 1)
    panic("unlink: nlink < 1");
  if(ip->type == T_DIR && !isdirempty(ip)){
    iunlockput(ip);
    iunlockput(dp);
    return -1;
  }

  memset(&de, 0, sizeof(de));
  if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
    panic("unlink: writei");
  if(ip->type == T_DIR){
    dp->nlink--;
    iupdate(dp);
  }
  iunlockput(dp);

  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);
  return 0;
}

static struct inode*
create(char *path, short type, short major, short minor)
{
  uint off;
  struct inode *ip, *dp;
  char name[DIRSIZ];

  if((dp = nameiparent(path, name)) == 0)
    return 0;
  ilock(dp);
  if((ip = dirlookup(dp, name, &off)) != 0){
    iunlockput(dp);
    ilock(ip);
    if(type == T_FILE && ip->type == T_FILE)
      return ip;
    iunlockput(ip);
    return 0;
  }
  if((ip = ialloc(dp->dev, type)) == 0)
    panic("create: ialloc");

  ilock(ip);
  ip->major = major;
  ip->minor = minor;
  ip->nlink = 1;
  iupdate(ip);

  if(type == T_DIR){  // Create . and .. entries.
    dp->nlink++;  // for ".."
    iupdate(dp);
    // No ip->nlink++ for ".": avoid cyclic ref count.
    if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0)
      panic("create dots");
  }

  if(dirlink(dp, name, ip->inum) < 0)
    panic("create: dirlink");

  iunlockput(dp);
  return ip;
}

int
sys_open(char *path, int omode)
{
  //char *path;
  int fd;
  struct file *f;
  struct inode *ip;

  if( omode < 0)
    return -1;
  if(omode & O_CREATE){
    if((ip = create(path, T_FILE, 0, 0)) == 0)
      return -1;
  } else {

    if((ip = namei(path)) == 0)
      return -1;
    ilock(ip);
    if(ip->type == T_DIR && omode != O_RDONLY){
      iunlockput(ip);
      return -1;
    }
  }

  if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
    if(f)
      fileclose(f);
    iunlockput(ip);
    return -1;
  }
  iunlock(ip);
  f->type = FD_INODE;
  f->ip = ip;
  f->off = 0;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);
  return fd;
}

int
sys_mkdir(char *path)
{
  //char *path;
  struct inode *ip;

  if(path < 0 || (ip = create(path, T_DIR, 0, 0)) == 0)
    return -1;
  iunlockput(ip);
  return 0;
}

int
sys_mknod(char *path, int major, int minor)
{
  struct inode *ip;
  //char *path;
  int len;
  //int major, minor;
  
  if(path < 0 ||
     major < 0 ||
     minor < 0 ||
     (ip = create(path, T_DEV, major, minor)) == 0)
    return -1;
  iunlockput(ip);
  return 0;
}

int
sys_chdir(char *path)
{
  //char *path;
  struct inode *ip;

  if(path < 0 || (ip = namei(path)) == 0)
    return -1;
  ilock(ip);
  if(ip->type != T_DIR){
    iunlockput(ip);
    return -1;
  }
  iunlock(ip);
  iput(proc->cwd);
  proc->cwd = ip;
  return 0;
}

int
sys_exec(char *path, char *uargv)
{
  //char *path, *argv[20];
  char *argv[20];
  int i;
  uint uarg;
  //path = upath;
  //for(i=0;; i++){
    //if(i >= NELEM(argv))
      //return -1;
 // if (*upath != 0)
  //{
 // *path= *upath;
  //path++;
 // upath++;}
  //else{
 // *path = 0;
  //break;
 // }
//}
  //if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0) {
  //  return -1;
  //}
  //memset(argv, 0, sizeof(argv));
  //*argv = *uargv;
  //for(i=0;; i++){
  //  if(i >= NELEM(argv))
  //    return -1;
  //if (*uargv != 0)
  //{
  //argv[i]= uargv[i];
  //else{
  //argv[i] = 0;
 // break;
  //}
  //  if(fetchint(proc, uargv+4*i, (int*)&uarg) < 0)
  //    return -1;
  //  if(uarg == 0){
   //   argv[i] = 0;
   //   break;
   // }
    //if(fetchstr(proc, uarg, &argv[i]) < 0)
   //   return -1;
  //}
  switchkvm();
  i = exec(path, uargv);
//  	asm volatile ("mov %0, r10\n\t"
//	"mov.l 11f, r0\n\t"
//	"add r0, r10\n\t"
//	"mov   #84, r0\n\t"
//	"mov.l %0, @-r10\n\t"
	
//"1:	cmp/eq #0, r0\n\t"
//	"bt 2f\n\t"

//	"!mov   #84, r3\n\t"
//	"mov.l @(r0, r15), r3\n\t"
//	"mov.l r3, @-r10\n\t"
//	"add   #-4, r0\n\t"
//	"bf 1b\n\t"

//"2:	nop\n\t"
//	"nop\n\t"
//	"mov r10, r15\n\t"


//".align 4\n\t"
//"11:	.long 0x80000000\n\t"
//	: : "r"(proc->context->r15));
  //swtch_stack(proc->context->r15);
  //__asm__ __volatile__(
	//"mov   #108, r0\n\t"	
	//"mov.l %0, @(r0, r15)\n\t"
//	"mov   #88, r0\n\t"	
//	"mov.l %1, @(r0, r15)\n\t"
 //                       : : "r" (proc->context->r15), "r"(proc->context->spc)
 //                      );
   
   //uint ITLB = 0xf2000000;
   //set_val_in_p2(0xf6003f80, 0xa37bf00);
   //switchuvm(proc);

//swtch_stack(proc->context->r15);
   //exe_return();
  return i;
}

int
sys_pipe(int *fd)
{
  //int *fd;
  struct file *rf, *wf;
  int fd0, fd1;

  if(fd < 0)
    return -1;
  if(pipealloc(&rf, &wf) < 0)
    return -1;
  fd0 = -1;
  if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
    if(fd0 >= 0)
      proc->ofile[fd0] = 0;
    fileclose(rf);
    fileclose(wf);
    return -1;
  }
  fd[0] = fd0;
  fd[1] = fd1;
  return 0;
}
