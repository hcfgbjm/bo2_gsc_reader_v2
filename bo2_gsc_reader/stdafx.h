#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Comdlg32.lib")

#include "crc32.h"

#include "bo2_gsc_file.h"
#include "bo2_gsc_reader.h"

#include "vm_define.h"
#include "vm_opcode_table.h"
#include "vm_stack.h"
#include "vm_opcode.h"
#include "vm_opcode_decompile.h"