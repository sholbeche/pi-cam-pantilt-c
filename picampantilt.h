#define DEF_FILENAME "capture.jpg"
#define IMAGEPATH "../images/"

void cameraTask(char *task, char *argv);
void secondSleep(float seconds);
void fileCopy(char *sourceFile, char *targetFile);
char * fileDateStamp();
char *replace_str2(const char *str, const char *old, const char *new);
