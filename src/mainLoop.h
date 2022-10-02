#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

typedef struct Callbacks {
    void (* initCallback)(void);
    void (* updateCallback) (void);
    void (* drawCallback) (void);
    void (* deinitCallback) (void);
} Callbacks;

void SetupCallbacks(const Callbacks * callbacks);
void ResetGame();
void DoMainLoop();

extern const Callbacks Term_Callbacks;    // Terminal

extern int breakGame;

#endif//MAIN_LOOP_H_
