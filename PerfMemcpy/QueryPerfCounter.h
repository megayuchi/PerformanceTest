#pragma once

void QCInit();
LARGE_INTEGER QCGetCounter();
float QCMeasureElapsedTick(LARGE_INTEGER CurCounter, LARGE_INTEGER PrvCounter);

