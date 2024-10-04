/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2024 MuseScore Limited
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
#pragma once

#include "types/fraction.h"
#include "dom/interval.h"
#include "musicxmltypes.h"
#include "musicxmlvoicedesc.h"

namespace mu::engraving {
class MusicXMLOctaveShiftList : public std::map<Fraction, int>
{
public:
    MusicXMLOctaveShiftList() {}
    int octaveShift(const Fraction f) const;
    void addOctaveShift(const int shift, const Fraction f);
    void calcOctaveShiftShifts();
};

class MusicXMLPart
{
public:
    MusicXMLPart(String id = {}, String name = {});
    void addMeasureNumberAndDuration(String measureNumber, Fraction measureDuration);
    String getId() const { return m_id; }
    String toString() const;
    VoiceList voicelist;           // the voice map information TODO: make private
    Fraction measureDuration(size_t i) const;
    size_t nMeasures() const { return m_measureDurations.size(); }
    MusicXMLInstrList _instrList;   // TODO: make private
    MusicXMLIntervalList _intervals;                       ///< Transpositions
    Interval interval(const Fraction f) const;
    int octaveShift(const staff_idx_t staff, const Fraction f) const;
    void addOctaveShift(const staff_idx_t staff, const int shift, const Fraction f);
    void calcOctaveShifts();
    void setName(String nm) { m_name = nm; }
    String getName() const { return m_name; }
    void setPrintName(const bool b) { m_printName = b; }
    bool getPrintName() const { return m_printName; }
    void setAbbr(String ab) { m_abbr = ab; }
    String getAbbr() const { return m_abbr; }
    void setPrintAbbr(const bool b) { m_printAbbr = b; }
    bool getPrintAbbr() const { return m_printAbbr; }
    std::map<int, int> staffNumberToIndex() const { return m_staffNumberToIndex; }
    int staffNumberToIndex(const int staffNumber) const;
    void insertStaffNumberToIndex(const int staffNumber, const int staffIndex) { m_staffNumberToIndex.insert({ staffNumber, staffIndex }); }
    LyricNumberHandler& lyricNumberHandler() { return m_lyricNumberHandler; }
    const LyricNumberHandler& lyricNumberHandler() const { return m_lyricNumberHandler; }
    void setMaxStaff(const int staff);
    int maxStaff() const { return m_maxStaff; }
    bool isVocalStaff() const;
    bool isPercussionStaff() const;
    void hasLyrics(bool b) { m_hasLyrics = b; }
private:
    String m_id;
    String m_name;
    bool m_printName = true;
    String m_abbr;
    bool m_printAbbr = false;
    std::vector<String> m_measureNumbers;               // MusicXML measure number attribute
    std::vector<Fraction> m_measureDurations;         // duration in fraction for every measure
    std::vector<MusicXMLOctaveShiftList> m_octaveShifts;   // octave shift list for every staff
    LyricNumberHandler m_lyricNumberHandler;
    int m_maxStaff = -1;                      // maximum staff value found (0 based), -1 = none
    bool m_hasLyrics = false;
    std::map<int, int> m_staffNumberToIndex;       // Mapping from staff number to index in staff list.
    // Only for when staves are discarded in MusicXMLParserPass1::attributes.
};
}
