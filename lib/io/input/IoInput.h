#pragme once

#define MASK_BOUNCE_STATE(n) (1<<(n))

typedef uint8_t DebounceState;

enum {DebouncedState, DebouncedStateUnstable, DebouncedStateChanged};

typedef enum {DebounceModeStable, DebounceModeLockout, DebounceModePromptDetection} DebounceMode;
