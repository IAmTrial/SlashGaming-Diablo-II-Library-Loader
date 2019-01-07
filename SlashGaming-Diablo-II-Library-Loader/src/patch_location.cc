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

#include "patch_location.h"

#include <memory>

#include <sgd2mapi.h>

namespace sgd2ll {

const sgd2mapi::GameAddress&
GetPatchAddress(
    void
) {
  static const sgd2mapi::GameAddress patch_address(
      sgd2mapi::DefaultLibrary::kD2Win,
      {
          {
              sgd2mapi::GameVersion::k1_13D,
              std::make_shared<sgd2mapi::GameOffset>(0xE134)
          }
      }
  );

  return patch_address;
}

}
