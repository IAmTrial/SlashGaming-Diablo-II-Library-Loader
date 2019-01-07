/**
 * SlashGaming Diablo II Library Loader
 * Copyright (C) 2019  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Library Loader.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permissions under GNU Affero General Public License version 3
 *  section 7
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with Diablo II (or a modified version of that game and its
 *  libraries), containing parts covered by the terms of Blizzard End User
 *  License Agreement, the licensors of this Program grant you additional
 *  permission to convey the resulting work.  This additional permission is
 *  also extended to any combination of expansions, mods, and remasters of
 *  the game.
 */

#include "library_loader.h"

#include <windows.h>
#include <string.h>

#include "asm_x86_macro.h"
#include "../include/extern_export.h"
#include "config_reader.h"
#include "patch_location.h"
#include "reverse_game_branch_patch.h"

namespace sgd2ll {
namespace {

void
LoadLibraries(
    void
) {
  // Load all of the libraries.
  std::vector libraries_path = GetLibrariesPaths();
  for (const auto& library_path : libraries_path) {
    LoadLibraryW(
        library_path.c_str()
    );
  }

  GetLibraryLoaderPatch().Remove();
}

} // namespace

__declspec(naked) void
LoadLibraries_Stub(
    void
) {
  // Load the libraries.
  ASM_X86(call LoadLibraries)
  ASM_X86(ret)

  /*
  // Allocate and copy the string into the stack.
  ASM_X86(sub esp, 0x800)
  ASM_X86(mov ecx, esp)
  ASM_X86(push GetGameExecutableFileName)
  ASM_X86(push ecx)
  ASM_X86(call strcpy)

  // Set up the return to clear the allocated space on the stack.
  ASM_X86(push GetStackClearRawAddress)

  // Unload the library.
  ASM_X86(jmp FreeLibraryA)

  // When FreeLibrary completes,
  */
}

} // namespace sgd2ll
