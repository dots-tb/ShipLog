int shipLogBufLength();
void copyFromUserToBuf(const void *buf, size_t length);
void shipLogBufClear();
int shipLogWriteCB(const void *buf, int size);