/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "mscxcompat.h"

#include <QFile>
#include <QBuffer>

#include "log.h"

Ms::Score::FileError mu::engraving::compat::loadMsczOrMscx(Ms::MasterScore* score, const QString& path, bool ignoreVersionError)
{
    QByteArray msczData;
    if (path.endsWith("mscx")) {
        //! NOTE Convert mscx -> mscz

        QFile mscxFile(path);
        if (!mscxFile.open(QIODevice::ReadOnly)) {
            LOGE() << "failed open file: " << path;
            return Ms::Score::FileError::FILE_OPEN_ERROR;
        }

        QByteArray mscxData = mscxFile.readAll();

        QBuffer buf(&msczData);
        MsczWriter writer(&buf);
        writer.setFilePath(path + ".mscz");
        writer.open();
        writer.writeScore(mscxData);
    } else {
        QFile msczFile(path);
        if (!msczFile.open(QIODevice::ReadOnly)) {
            LOGE() << "failed open file: " << path;
            return Ms::Score::FileError::FILE_OPEN_ERROR;
        }

        msczData = msczFile.readAll();
    }

    QBuffer msczBuf(&msczData);
    MsczReader reader(&msczBuf);
    reader.open();

    Ms::Score::FileError err = score->loadMscz(reader, ignoreVersionError);
    return err;
}
