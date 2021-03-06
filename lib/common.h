/*
 * common.h: EPG2VDR plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <stdint.h>   // uint_64_t
#include <stdlib.h>
#include <iconv.h>
#include <errno.h>
#include <string.h>

#include <string>

#include <openssl/md5.h> // MD5_*

struct MemoryStruct;
extern int loglevel;
extern int logstdout;
extern int logstamp;

using namespace std;

typedef unsigned char byte;
typedef unsigned short word;
typedef short sword;
typedef unsigned int dword;

//***************************************************************************
// 
//***************************************************************************

inline long min(long a, long b) { return a < b ? a : b; }

enum Misc
{
   success = 0,
   done    = success,
   fail    = -1,
   na      = -1,
   ignore  = -2,
   all     = -3,
   abrt     = -4,
   yes     = 1,
   on      = 1,
   off     = 0,
   no      = 0,
   TB      = 1,

   sizeMd5 = 2 * MD5_DIGEST_LENGTH,

   tmeSecondsPerMinute = 60,
   tmeSecondsPerHour = 60 * tmeSecondsPerMinute,
   tmeSecondsPerDay = 24 * tmeSecondsPerHour
};

//***************************************************************************
// Tell
//***************************************************************************

enum Eloquence
{
   eloOff = na,              // -1
   eloAlways,                // 0
   eloDetail,                // 1
   eloDebug,                 // 2
   eloDebug2,                // 3
   eloDebug3                 // 4
};

void __attribute__ ((format(printf, 2, 3))) tell(int eloquence, const char* format, ...);

char* srealloc(void* ptr, size_t size);

//***************************************************************************
// MemoryStruct
//***************************************************************************

struct MemoryStruct
{
   public:

      MemoryStruct()   { expireAt = 0; memory = 0; zmemory = 0; clear(); }
      MemoryStruct(const MemoryStruct* o)
      {
         size = o->size;
         memory = (char*)malloc(size);
         memcpy(memory, o->memory, size);

         zsize = o->zsize;
         zmemory = (char*)malloc(zsize);
         memcpy(zmemory, o->zmemory, zsize);

         copyAttributes(o);
      }
      
      ~MemoryStruct()  { clear(); }

      int isEmpty()  { return memory == 0; }
      int isZipped() { return zmemory != 0 && zsize > 0; }

      int append(const char* buf, int len = 0)
      {
         if (!len)
            len = strlen(buf) + TB;
         
         memory = srealloc(memory, size+len);
         memcpy(memory+size, buf, len);
         size += len;

         return success;
      }

      void copyAttributes(const MemoryStruct* o)
      {
         strcpy(tag, o->tag);
         strcpy(name, o->name);
         strcpy(contentType, o->contentType);
         strcpy(contentEncoding, o->contentEncoding);
         strcpy(mimeType, o->mimeType);
         headerOnly = o->headerOnly;
         modTime = o->modTime;
         expireAt = o->expireAt;
      }
     
      void clear() 
      {
         free(memory);
         memory = 0;
         size = 0;
         free(zmemory);
         zmemory = 0;
         zsize = 0;
         *tag = 0;
         *name = 0;
         *contentType = 0;
         *contentEncoding = 0;
         *mimeType = 0;
         modTime = time(0);
         headerOnly = no;
         // expireAt = time(0); -> don't reset 'expireAt' here !!!!
      }

      // data
      
      char* memory;
      long unsigned int size;

      char* zmemory;
      long unsigned int zsize;
      
      // tag attribute
      
      char tag[100+TB];              // the tag to be compared 
      char name[100+TB];             // content name (filename)
      char contentType[100+TB];      // e.g. text/html
      char mimeType[100+TB];         // 
      char contentEncoding[100+TB];  // 
      int headerOnly;
      time_t modTime;
      time_t expireAt;
};

//***************************************************************************
// cMyMutex
//***************************************************************************

class cMyMutex 
{
   friend class cCondVar;

   public:

      cMyMutex(void);
      ~cMyMutex();
      void Lock(void);
      void Unlock(void);

   private:

      pthread_mutex_t mutex;
      int locked;
};

//***************************************************************************
// Tools
//***************************************************************************

double usNow();
unsigned int getHostId();
byte crc(const byte* data, int size);
int toUTF8(char* out, int outMax, const char* in, const char* from_code = 0);

void removeChars(std::string& str, const char* ignore);
void removeCharsExcept(std::string& str, const char* except);
void removeWord(std::string& pattern, std::string word);
void prepareCompressed(std::string& pattern);
string strReplace(const string& what, const string& with, const string& subject);
string strReplace(const string& what, long with, const string& subject);
string strReplace(const string& what, double with, const string& subject);

const char* plural(int n, const char* s = "s");
char* rTrim(char* buf);
char* lTrim(char* buf);
char* allTrim(char* buf);
int isNum(const char* value);
char* sstrcpy(char* dest, const char* src, int max);
string num2Str(int num);
string num2Str(double num);
string l2pTime(time_t t);
char* eos(char* s);
const char* toElapsed(int seconds, char* buf);

int fileExists(const char* path);
const char* suffixOf(const char* path);
int createLink(const char* link, const char* dest, int force);
int isLink(const char* path);
int isEmpty(const char* str);
int removeFile(const char* filename);
int loadFromFile(const char* infile, MemoryStruct* data);

const char* getHostName();
const char* getFirstIp();

#ifdef USEMD5
  typedef char md5Buf[sizeMd5+TB];
  typedef char md5;
  int createMd5(const char* buf, md5* md5);
  int createMd5OfFile(const char* path, const char* name, md5* md5);
#endif

#ifdef WITH_GUNZIP

//***************************************************************************
// Zip
//***************************************************************************

int gunzip(MemoryStruct* zippedData, MemoryStruct* unzippedData);
void tellZipError(int errorCode, const char* op, const char* msg);

#endif // WITH_GUNZIP

//***************************************************************************
// 
//***************************************************************************

class cRetBuf
{
   public:

      cRetBuf(const char* buf = 0);
      cRetBuf(const cRetBuf& buf);
      virtual ~cRetBuf();

      operator const void* () const { return s; } // to catch cases where operator*() should be used
      operator const char* () const { return s; } // for use in (const char *) context
      const char* operator*() const { return s; } // for use in (const void *) context (printf() etc.)
      cRetBuf& operator = (const cRetBuf& buf);
      cRetBuf& operator = (const char* buf);

   private:

      char* s;
};

//***************************************************************************
// cTimeMs 
//***************************************************************************

class cTimeMs 
{
   public:

      cTimeMs(int Ms = 0);
      static uint64_t Now();
      void Set(int Ms = 0);
      bool TimedOut();
      uint64_t Elapsed();

   private:

      uint64_t begin;
};

//***************************************************************************
// Semaphore
//***************************************************************************

#include <sys/sem.h>

class Sem
{
   public:

      Sem(int aKey)
      {
         locked = no;
         key = aKey;

         if ((id = semget(key, 1, 0666 | IPC_CREAT)) == -1)
            tell(eloAlways, "Error: Can't get semaphore, errno (%d) '%s'", 
                 errno, strerror(errno));
      }

      ~Sem() 
      { 
         if (locked)
            v();
      }

      // ----------------------
      // get lock

      int p()
      {
         sembuf sops[2];
         
         sops[0].sem_num = 0;
         sops[0].sem_op = 0;                        // wait for lock
         sops[0].sem_flg = SEM_UNDO;
         
         sops[1].sem_num = 0;
         sops[1].sem_op = 1;                        // increment 
         sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT;
         
         if (semop(id, sops, 2) == -1)
         {
            tell(eloAlways, "Error: Can't lock semaphore, errno (%d) '%s'", 
                 errno, strerror(errno));
            
            return fail;
         }

         locked = yes;

         return success;
      }
      
      int inc()
      {
         sembuf sops[1];
         
         sops[0].sem_num = 0;
         sops[0].sem_op = 1;                        // increment 
         sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
         
         if (semop(id, sops, 1) == -1)
         {
            if (errno != EAGAIN)
               tell(0, "Error: Can't lock semaphore, errno was (%d) '%s'", 
                    errno, strerror(errno));
            
            return fail;
         }
         
         locked = yes;

         return success;
      }

      // ----------------------
      // decrement

      int dec()
      {
         return v();
      }

      // ----------------------
      // check

      int check()
      {
         sembuf sops[1];
         
         sops[0].sem_num = 0;
         sops[0].sem_op = 0; 
         sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
         
         if (semop(id, sops, 1) == -1)
         {
            if (errno != EAGAIN)
               tell(0, "Error: Can't lock semaphore, errno was (%d) '%s'", 
                    errno, strerror(errno));
            
            return fail;
         }
         
         return success;
      }

      // ----------------------
      // release lock

      int v()
      {
         sembuf sops;
         
         sops.sem_num = 0;
         sops.sem_op = -1;                          // release control
         sops.sem_flg = SEM_UNDO | IPC_NOWAIT;
         
         if (semop(id, &sops, 1) == -1)
         {
            tell(eloAlways, "Error: Can't unlock semaphore, errno (%d) '%s'",
                 errno, strerror(errno));
            
            return fail;
         }
         
         locked = no;

         return success;
      }
      
   private:

      int key;
      int id;
      int locked;
};

//***************************************************************************
#endif // ___COMMON_H
