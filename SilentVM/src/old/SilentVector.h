#ifndef SILENT_VECTOR
#define SILENT_VECTOR

typedef struct SilentVector
{
    char* data;
    unsigned long long ptr;
    unsigned long long spaceLeft;
    unsigned long long vectorSize;
    unsigned long long dataSize;
    unsigned long long reallocSize;
}SilentVector;

SilentVector* SilentCreateVector(
    unsigned long long reallocSize,
    unsigned long long dataSize
);

void SilentTrimVector(SilentVector* vector);
//void SilentPushBack(SilentVector* vector, void* data);
//void SilentPushMultiple(
//    SilentVector* vector, unsigned long long count, void* data
//);
//void SilentPopBack(SilentVector* vector);
//void SilentPopMultiple(SilentVector* vector, unsigned long long count);
void SilentDeleteVector(SilentVector* vector);
#endif