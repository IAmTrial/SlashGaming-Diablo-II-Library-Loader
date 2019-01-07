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

#include "safe_game_branch_patch.h"

#include <memory>

#include <sgd2mapi.h>
#include "patch_location.h"

namespace sgd2ll {

ReverseGameBranchPatch::ReverseGameBranchPatch(
    const sgd2mapi::GameAddress& game_address,
    void (*func)(void),
    std::size_t patch_size
)
    : GameBranchPatch(
          game_address,
          sgd2mapi::BranchType::kJump,
          func,
          patch_size
      ) {
}

void ReverseGameBranchPatch::Apply() {
  // Construct the branch patch.
  std::vector<BYTE> branch_patch(sizeof(std::intptr_t) + 1);
  branch_patch[0] = static_cast<BYTE>(branch_type());

  for (int i = 0; i < sizeof(func_ptr()); i += 1) {
    int shift_amount = (i * 8);
    branch_patch[i + 1] = static_cast<BYTE>(
        (func_ptr() >> shift_amount) & 0xFF
    );
  }

  std::size_t nop_patch_size =
      (patch_size() - branch_patch.size());

  std::intptr_t branch_address =
      (game_address().address() + nop_patch_size);

  // Start by patching in the function (in an unreachable area).
  WriteProcessMemory(
      GetCurrentProcess(),
      reinterpret_cast<LPVOID>(branch_address),
      branch_patch.data(),
      branch_patch.size(),
      nullptr
  );

  // Patch in a NOP to turn our unreachable code into reachable.
  std::vector<BYTE> nop_patch(
      nop_patch_size,
      0x90
  );
  WriteProcessMemory(
      GetCurrentProcess(),
      reinterpret_cast<LPVOID>(game_address().address()),
      nop_patch.data(),
      nop_patch.size(),
      nullptr
  );
}

} // namespace sgd2ll
