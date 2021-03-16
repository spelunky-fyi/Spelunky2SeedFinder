#include "SeedFinderUtil.h"
#include "SeedFinder.h"
#include "state.hpp"
#include <iostream>
#include <sstream>

namespace SeedFinder
{
    // ************** UTIL **************
    std::ofstream Util::msLogStream;

    void Util::log(const std::string& s)
    {
        if (msLogStream.is_open())
        {
            msLogStream << s << "\n";
        }
    }

    std::string Util::joinVectorOfStrings(const std::vector<std::string>& v, const char* delimiter)
    {
        switch (v.size())
        {
        case 0:
            return "";
        case 1:
            return v.at(0);
        default:
            std::ostringstream os;
            std::copy(v.begin(), v.end() - 1, std::ostream_iterator<std::string>(os, delimiter));
            os << *v.rbegin();
            return os.str();
        }
    }

    void Util::startLogging() { msLogStream = std::ofstream("seed-finder.log", std::ios::app); }

    void Util::stopLogging() { msLogStream.close(); }

    // ************** RECT **************
    Rect::Rect() : x(0), y(0), width(0), height(0) {}

    Rect::Rect(float xx, float yy, float ww, float hh) : x(xx), y(yy), width(ww), height(hh) {}

    bool Rect::containsPoint(float xx, float yy) const { return (xx >= x && xx <= x + width && yy >= y && yy <= y + height); }

    bool Rect::isValid() const { return (width != 0 && height != 0); }

    // ************** LEVEL STORAGE **************
    void LevelStorage::all()
    {
        // clang-format off
    if (!isHidden(1, 1)) { OneOne = true; }
    if (!isHidden(1, 2)) { OneTwo = true; }
    if (!isHidden(1, 3)) { OneThree = true; }
    if (!isHidden(1, 4)) { Quillback = true; }
    if (!isHidden(2, 1)) { TwoOne = true; }
    if (!isHidden(2, 2)) { TwoTwo = true; }
    if (!isHidden(2, 3)) { TwoThree = true; }
    if (!isHidden(2, 4)) { TwoFour = true; }
    if (!isHidden(3, 1)) { Olmec = true; }
    if (!isHidden(4, 1)) { FourOne = true; }
    if (!isHidden(4, 2)) { FourTwo = true; }
    if (!isHidden(4, 3)) { FourThree = true; }
    if (!isHidden(4, 4)) { FourFour = true; }
    if (!isHidden(5, 1)) { IceCaves = true; }
    if (!isHidden(6, 1)) { SixOne = true; }
    if (!isHidden(6, 2)) { SixTwo = true; }
    if (!isHidden(6, 3)) { SixThree = true; }
    if (!isHidden(6, 4)) { Tiamat = true; }
    if (!isHidden(7, 1)) { SevenOne = true; }
    if (!isHidden(7, 2)) { SevenTwo = true; }
    if (!isHidden(7, 3)) { SevenThree = true; }
    if (!isHidden(7, 4)) { Hundun = true; }
        // clang-format on
    }

    void LevelStorage::none()
    {
        // clang-format off
        OneOne = false; OneTwo = false; OneThree = false; Quillback = false;
        TwoOne = false; TwoTwo = false; TwoThree = false; TwoFour = false;
        Olmec = false; 
        FourOne = false; FourTwo = false; FourThree = false; FourFour = false;
        IceCaves = false;
        SixOne = false; SixTwo = false; SixThree = false; Tiamat = false;
        SevenOne = false; SevenTwo = false; SevenThree = false; Hundun = false;
        Any = false;
        // clang-format on
    }

    uint8_t LevelStorage::deepest() const
    {
        // clang-format off
        if ( Any ) { return 99; }
        if ( Hundun ) { return 21; }    if ( SevenThree ) { return 20; }    if ( SevenTwo ) { return 19; }  if ( SevenOne ) { return 18; }
        if ( Tiamat ) { return 17; }    if ( SixThree ) { return 16; }      if ( SixTwo ) { return 15; }    if ( SixOne ) { return 14; }
        if ( IceCaves ) { return 13; }  
        if ( FourFour ) { return 12; }  if ( FourThree ) { return 11; }     if ( FourTwo ) { return 10; }   if ( FourOne ) { return 9; }
        if ( Olmec ) { return 8; }
        if ( TwoFour ) { return 7; }    if ( TwoThree ) { return 6; }       if ( TwoTwo ) { return 5; }     if ( TwoOne ) { return 4; }
        if ( Quillback ) { return 3; }    if ( OneThree ) { return 2; }       if ( OneTwo ) { return 1; }     if ( OneOne ) { return 0; }
        return 0;
        // clang-format on
    }

    void LevelStorage::activateDepth(uint8_t depth)
    {
        // clang-format off
    switch( depth )
    {
        case 0: { OneOne = true; break; }
        case 1: { OneTwo = true; break; }
        case 2: { OneThree = true; break; }
        case 3: { Quillback = true; break; }
        case 4: { TwoOne = true; break; }
        case 5: { TwoTwo = true; break; }
        case 6: { TwoThree = true; break; }
        case 7: { TwoFour = true; break; }
        case 8: { Olmec = true; break; }
        case 9: { FourOne = true; break; }
        case 10: { FourTwo = true; break; }
        case 11: { FourThree = true; break; }
        case 12: { FourFour = true; break; }
        case 13: { IceCaves = true; break; }
        case 14: { SixOne = true; break; }
        case 15: { SixTwo = true; break; }
        case 16: { SixThree = true; break; }
        case 17: { Tiamat = true; break; }
        case 18: { SevenOne = true; break; }
        case 19: { SevenTwo = true; break; }
        case 20: { SevenThree = true; break; }
        case 21: { Hundun = true; break; }
        case 99: { Any = true; break; }
    }
        // clang-format on
    }

    bool LevelStorage::isValid() const
    {
        // clang-format off
        return OneOne || OneTwo || OneThree || Quillback || TwoOne || TwoTwo || TwoThree || TwoFour || Olmec || FourOne || FourTwo || FourThree
                || FourFour || IceCaves || SixOne || SixTwo || SixThree || Tiamat || SevenOne || SevenTwo || SevenThree || Hundun || Any;
        // clang-format on
    }

    std::vector<std::string> LevelStorage::chosenLevels() const
    {
        std::vector<std::string> l;
        // clang-format off
    if (OneOne) { l.emplace_back("1-1"); } if (OneTwo) { l.emplace_back("1-2"); } if (OneThree) { l.emplace_back("1-3"); } if (Quillback) { l.emplace_back("1-4 (Quillback)"); }
    if (TwoOne) { l.emplace_back("2-1"); } if (TwoTwo) { l.emplace_back("2-2"); } if (TwoThree) { l.emplace_back("2-3"); } if (TwoFour) { l.emplace_back("2-4"); }
    if (Olmec) { l.emplace_back("3-1 (Olmec)"); }
    if (FourOne) { l.emplace_back("4-1"); } if (FourTwo) { l.emplace_back("4-2"); } if (FourThree) { l.emplace_back("4-3"); } if (FourFour) { l.emplace_back("4-4"); }
    if (IceCaves) { l.emplace_back("5-1 (IceCaves)"); }
    if (SixOne) { l.emplace_back("6-1"); } if (SixTwo) { l.emplace_back("6-2"); } if (SixThree) { l.emplace_back("6-3"); } if (Tiamat) { l.emplace_back("6-4 (Tiamat)"); }
    if (SevenOne) { l.emplace_back("7-1"); } if (SevenTwo) { l.emplace_back("7-2"); } if (SevenThree) { l.emplace_back("7-3"); } if (Hundun) { l.emplace_back("7-4 (Hundun)"); }
    if (Any) { l.emplace_back("Any"); }
        // clang-format on
        return l;
    }

    bool LevelStorage::shouldExecute(uint8_t world, uint8_t level) const
    {
        // clang-format off
    if (world == 1 && level == 1 && OneOne) { return true; }
    if (world == 1 && level == 2 && OneTwo) { return true; }
    if (world == 1 && level == 3 && OneThree) { return true; }
    if (world == 1 && level == 4 && Quillback) { return true; }
    if (world == 2 && level == 1 && TwoOne) { return true; }
    if (world == 2 && level == 2 && TwoTwo) { return true; }
    if (world == 2 && level == 3 && TwoThree) { return true; }
    if (world == 2 && level == 4 && TwoFour) { return true; }
    if (world == 3 && level == 1 && Olmec) { return true; }
    if (world == 4 && level == 1 && FourOne) { return true; }
    if (world == 4 && level == 2 && FourTwo) { return true; }
    if (world == 4 && level == 3 && FourThree) { return true; }
    if (world == 4 && level == 4 && FourFour) { return true; }
    if (world == 5 && level == 1 && IceCaves) { return true; }
    if (world == 6 && level == 1 && SixOne) { return true; }
    if (world == 6 && level == 2 && SixTwo) { return true; }
    if (world == 6 && level == 3 && SixThree) { return true; }
    if (world == 6 && level == 4 && Tiamat) { return true; }
    if (world == 7 && level == 1 && SevenOne) { return true; }
    if (world == 7 && level == 2 && SevenTwo) { return true; }
    if (world == 7 && level == 3 && SevenThree) { return true; }
    if (world == 7 && level == 4 && Hundun) { return true; }
    if (Any) { return true; }
        // clang-format on
        return false;
    }

    void LevelStorage::hideLevel(uint8_t level) { hiddenLevels.insert(level); }

    void LevelStorage::disableLevel(uint8_t level) { disabledLevels.insert(level); }

    bool LevelStorage::isHidden(uint8_t level) { return (hiddenLevels.count(level) > 0); }

    bool LevelStorage::isDisabled(uint8_t level) { return (disabledLevels.count(level) > 0); }

    void LevelStorage::hideLevel(uint8_t world, uint8_t level)
    {
        // clang-format off
    if (world == 1 && level == 1) { hideLevel(0); }
    if (world == 1 && level == 2) { hideLevel(1); }
    if (world == 1 && level == 3) { hideLevel(2); }
    if (world == 1 && level == 4) { hideLevel(3); }
    if (world == 2 && level == 1) { hideLevel(4); }
    if (world == 2 && level == 2) { hideLevel(5); }
    if (world == 2 && level == 3) { hideLevel(6); }
    if (world == 2 && level == 4) { hideLevel(7); }
    if (world == 3 && level == 1) { hideLevel(8); }
    if (world == 4 && level == 1) { hideLevel(9); }
    if (world == 4 && level == 2) { hideLevel(10); }
    if (world == 4 && level == 3) { hideLevel(11); }
    if (world == 4 && level == 4) { hideLevel(12); }
    if (world == 5 && level == 1) { hideLevel(13); }
    if (world == 6 && level == 1) { hideLevel(14); }
    if (world == 6 && level == 2) { hideLevel(15); }
    if (world == 6 && level == 3) { hideLevel(16); }
    if (world == 6 && level == 4) { hideLevel(17); }
    if (world == 7 && level == 1) { hideLevel(18); }
    if (world == 7 && level == 2) { hideLevel(19); }
    if (world == 7 && level == 3) { hideLevel(20); }
    if (world == 7 && level == 4) { hideLevel(21); }
        // clang-format on
    }

    void LevelStorage::disableLevel(uint8_t world, uint8_t level)
    {
        // clang-format off
    if (world == 1 && level == 1) { disableLevel(0); }
    if (world == 1 && level == 2) { disableLevel(1); }
    if (world == 1 && level == 3) { disableLevel(2); }
    if (world == 1 && level == 4) { disableLevel(3); }
    if (world == 2 && level == 1) { disableLevel(4); }
    if (world == 2 && level == 2) { disableLevel(5); }
    if (world == 2 && level == 3) { disableLevel(6); }
    if (world == 2 && level == 4) { disableLevel(7); }
    if (world == 3 && level == 1) { disableLevel(8); }
    if (world == 4 && level == 1) { disableLevel(9); }
    if (world == 4 && level == 2) { disableLevel(10); }
    if (world == 4 && level == 3) { disableLevel(11); }
    if (world == 4 && level == 4) { disableLevel(12); }
    if (world == 5 && level == 1) { disableLevel(13); }
    if (world == 6 && level == 1) { disableLevel(14); }
    if (world == 6 && level == 2) { disableLevel(15); }
    if (world == 6 && level == 3) { disableLevel(16); }
    if (world == 6 && level == 4) { disableLevel(17); }
    if (world == 7 && level == 1) { disableLevel(18); }
    if (world == 7 && level == 2) { disableLevel(19); }
    if (world == 7 && level == 3) { disableLevel(20); }
    if (world == 7 && level == 4) { disableLevel(21); }
        // clang-format on
    }

    bool LevelStorage::isHidden(uint8_t world, uint8_t level)
    {
        // clang-format off
    if (world == 1 && level == 1) { return isHidden(0); }
    if (world == 1 && level == 2) { return isHidden(1); }
    if (world == 1 && level == 3) { return isHidden(2); }
    if (world == 1 && level == 4) { return isHidden(3); }
    if (world == 2 && level == 1) { return isHidden(4); }
    if (world == 2 && level == 2) { return isHidden(5); }
    if (world == 2 && level == 3) { return isHidden(6); }
    if (world == 2 && level == 4) { return isHidden(7); }
    if (world == 3 && level == 1) { return isHidden(8); }
    if (world == 4 && level == 1) { return isHidden(9); }
    if (world == 4 && level == 2) { return isHidden(10); }
    if (world == 4 && level == 3) { return isHidden(11); }
    if (world == 4 && level == 4) { return isHidden(12); }
    if (world == 5 && level == 1) { return isHidden(13); }
    if (world == 6 && level == 1) { return isHidden(14); }
    if (world == 6 && level == 2) { return isHidden(15); }
    if (world == 6 && level == 3) { return isHidden(16); }
    if (world == 6 && level == 4) { return isHidden(17); }
    if (world == 7 && level == 1) { return isHidden(18); }
    if (world == 7 && level == 2) { return isHidden(19); }
    if (world == 7 && level == 3) { return isHidden(20); }
    if (world == 7 && level == 4) { return isHidden(21); }
        // clang-format on
        return false;
    }

    bool LevelStorage::isDisabled(uint8_t world, uint8_t level)
    {
        // clang-format off
    if (world == 1 && level == 1) { return isDisabled(0); }
    if (world == 1 && level == 2) { return isDisabled(1); }
    if (world == 1 && level == 3) { return isDisabled(2); }
    if (world == 1 && level == 4) { return isDisabled(3); }
    if (world == 2 && level == 1) { return isDisabled(4); }
    if (world == 2 && level == 2) { return isDisabled(5); }
    if (world == 2 && level == 3) { return isDisabled(6); }
    if (world == 2 && level == 4) { return isDisabled(7); }
    if (world == 3 && level == 1) { return isDisabled(8); }
    if (world == 4 && level == 1) { return isDisabled(9); }
    if (world == 4 && level == 2) { return isDisabled(10); }
    if (world == 4 && level == 3) { return isDisabled(11); }
    if (world == 4 && level == 4) { return isDisabled(12); }
    if (world == 5 && level == 1) { return isDisabled(13); }
    if (world == 6 && level == 1) { return isDisabled(14); }
    if (world == 6 && level == 2) { return isDisabled(15); }
    if (world == 6 && level == 3) { return isDisabled(16); }
    if (world == 6 && level == 4) { return isDisabled(17); }
    if (world == 7 && level == 1) { return isDisabled(18); }
    if (world == 7 && level == 2) { return isDisabled(19); }
    if (world == 7 && level == 3) { return isDisabled(20); }
    if (world == 7 && level == 4) { return isDisabled(21); }
        // clang-format on
        return false;
    }

} // namespace SeedFinder