#pragma once

// Prevents debuggers from attaching to the current thread.
void HideThreadFromDebugger(HANDLE toHide = GetCurrentThread());