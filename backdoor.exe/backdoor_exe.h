//shipLogBufLength()
//Returns current position or length of the contents in buffer
int shipLogBufLength();

//shipLogBufClear()
//Clears the buffer and resets position to 0
void shipLogBufClear();

//shipLogWriteCB(const void *buf, int size)
//NO IDEA HOW THIS ONE WILL WORK, need to fix it
int shipLogWriteCB(const void *buf, int size);

//shipLogcpyBufToUsr(const void *dest, int pos, int length)
//Copies data from the log buffer to a user space buffer.
// dest : the destination buffer in user space
// pos : starting point to which data will be copied (from the log buffer) in kernel space
// length : the length desired or to be copied
int shipLogcpyBufToUsr(const void *dest, int pos, int length);

//shipLogDumpToDisk();
//Dumps the log buffer to default location
void shipLogDumpToDisk();
