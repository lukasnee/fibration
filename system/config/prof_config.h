// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

// First of the 12 ITM ports, default 0
#define LN_PROF_ITM_BASE_PORT 0
// Bytes of code from the start of flash that can be profiled, default 0x80000
#define LN_PROF_FW_SIZE 0x80000
// Section for the event hot path, default none
#define LN_PROF_HOT_SECTION ".ccmram"
// Longest LN_PROF_MSG() text, at most 255, default 64
#define LN_PROF_MSG_MAX_SIZE 64
// Compile LN_PROF_MSG() out
// #define LN_PROF_NO_MESSAGES
