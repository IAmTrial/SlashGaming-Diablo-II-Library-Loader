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

#include <windows.h>
#include <memory>
#include <mutex>
#include <string>

#include <sgd2mapi.h>
#include "library_loader.h"
#include "patch_location.h"
#include "patch_mutex.h"
#include "reverse_game_branch_patch.h"

namespace sgd2ll {

namespace {

bool
IsIntendedProcess(
    void
) {
  HANDLE current_process_handle = GetCurrentProcess();
  std::unique_ptr current_proc_name = std::make_unique<CHAR[]>(1024);

  DWORD current_process_file_length = GetModuleFileNameA(
      nullptr,
      current_proc_name.get(),
      1024
  );

  current_proc_name[1023] = '\0';

  int compare_result = _stricmp(
      current_proc_name.get()
          + current_process_file_length
          - sgd2mapi::kGameExecutable.length(),
      sgd2mapi::kGameExecutable.data()
  );

  return (compare_result == 0);
}

} // namespace

extern "C" BOOL WINAPI
DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
) {
  // Check that this is the process we seek to inject in.
  if (!IsIntendedProcess()) {
    return TRUE;
  }

  std::lock_guard lock(GetPatchMutex());

  switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
      GetLibraryLoaderPatch().Apply();
      return TRUE;
    }

    case DLL_PROCESS_DETACH: {
      GetLibraryLoaderPatch().Remove();
      return TRUE;
    }

    default: {
      return FALSE;
    }
  }
}

} // namespace sgd2ll
