/*
  Weiss is a UCI compliant chess engine.
  Copyright (C) 2020  Terje Kirstihagen

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "board.h"
#include "evaluate.h"
#include "psqt.h"


extern const int PieceTypeValue[7];


int PSQT[PIECE_NB][64];

// Black's point of view - easier to read as it's not upside down
const int PieceSqValue[6][64] = {

    { S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0),
      S( 62, 64), S( 49, 64), S( 56, 36), S( 55,  2), S( 56,  0), S( 47, 18), S(-36, 62), S(-37, 66),
      S(  8, 83), S( 35, 68), S( 48, 35), S( 49, -7), S( 67,-11), S(110, 14), S( 70, 41), S( 22, 56),
      S(  0, 36), S( -5, 23), S(  0,  7), S( 18,-25), S( 25,-19), S( 32, -9), S(  0, 14), S( -1, 17),
      S( -9, 12), S(-15,  9), S( -5,-11), S( -4,-17), S(  6,-18), S(  2,-10), S( -7, -2), S( -7, -4),
      S(-16,  3), S(-24, -2), S(-22, -2), S(-17, -9), S(-11, -2), S(-13,  4), S( -5,-11), S( -7,-13),
      S( -7,  7), S( -9,  5), S(-14, 10), S( -8,  8), S(-14, 17), S(  9, 13), S( 15, -3), S(  0,-21),
      S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0), S(  0,  0) },

    { S(-125,-91), S(-41,-22), S(-66, 14), S(-22, -1), S( -5,  2), S(-53, 15), S(-42,-16), S(-99,-77),
      S( -9,-26), S(-14, -2), S( 42,-13), S( 61, 14), S( 52, 11), S( 55,-24), S(-29, -2), S(-16,-22),
      S(-24,-18), S( 20,  2), S( 28, 42), S( 46, 37), S( 78, 20), S( 57, 33), S( 34, -6), S(-17,-15),
      S(  5, -9), S( 16, 14), S( 31, 46), S( 26, 59), S( 18, 57), S( 37, 38), S( 14, 18), S( 12, -6),
      S(  2, -5), S( 19,  9), S( 14, 45), S( 18, 48), S( 21, 44), S( 20, 43), S( 36,  8), S( 23,  5),
      S(-22,-43), S( -6,-12), S( -7, 10), S(  6, 28), S(  7, 26), S(  0,  7), S(  1, -6), S( -6,-33),
      S(-25,-18), S(-41, -1), S(-19,-19), S( -6,  5), S(-11,  1), S(-17,-20), S(-30, -9), S( -8, -5),
      S(-52,-52), S(-13,-47), S(-15,-28), S(-11,  3), S( -4,  4), S(-10,-22), S(-13,-27), S(-51,-52) },

    { S(-40, 33), S(-29, 24), S(-58, 24), S(-59, 33), S(-54, 30), S(-79, 19), S(-17, 16), S(-28, 19),
      S(-39, 26), S( 11, 25), S(  6, 22), S(-17, 24), S( -1, 19), S( -3, 25), S(-13, 27), S(-53, 31),
      S(  6, 12), S( 27, 20), S( 56, 13), S( 35, 12), S( 48, 11), S( 47, 21), S( 23, 23), S(  8,  7),
      S( -4,  7), S( 33, 11), S( 23, 12), S( 49, 22), S( 39, 25), S( 21, 18), S( 31, 12), S( -7, 11),
      S(  7,-12), S( 17, -4), S( 18, 18), S( 30, 19), S( 31, 19), S( 16, 14), S( 18,  0), S( 20,-13),
      S( 10,-17), S( 22,  2), S( 15,  8), S( 14, 14), S( 15, 14), S( 20,  6), S( 28, -6), S( 28,-11),
      S( 21,-24), S( 19,-33), S( 10,-11), S(  1,  1), S(  2, -1), S(  7,-18), S( 23,-32), S( 22,-49),
      S( 27,-18), S( 15,  5), S( 12,  1), S(  5, -4), S(  5,  3), S( 10, -6), S( 11,  6), S( 25,-23) },

    { S( 39, 38), S( 32, 46), S(  8, 58), S( 13, 53), S( 17, 52), S( 17, 54), S( 35, 46), S( 40, 44),
      S(  1, 43), S(-10, 53), S( 16, 49), S( 23, 53), S( 15, 54), S( 31, 27), S( -4, 40), S(  6, 35),
      S(  4, 36), S( 51, 16), S( 33, 33), S( 61, 14), S( 75,  3), S( 61, 19), S( 79, -4), S( 23, 24),
      S( -2, 28), S( 14, 27), S( 24, 29), S( 49, 16), S( 39, 16), S( 32, 14), S( 30, 12), S( 16, 18),
      S(-21, 15), S(-17, 29), S(-18, 30), S(-12, 21), S(-13, 16), S(-18, 20), S( -1, 15), S(-14,  8),
      S(-30, -6), S(-16,  5), S(-27,  3), S(-18, -4), S(-21, -5), S(-23, -7), S( -3, -9), S(-24,-14),
      S(-57,  3), S(-21, -9), S(-15, -4), S( -9,-10), S( -8,-14), S(-15,-21), S(-13,-23), S(-62, -3),
      S(-22,  2), S(-16,  2), S(-14,  3), S( -6, -9), S(-10,-10), S( -9,  0), S( -5, -7), S(-15,-13) },

    { S( -2, 15), S(  9, 34), S( 17, 44), S( 26, 60), S( 30, 71), S( 48, 67), S( 25, 57), S( 28, 48),
      S(-17, 14), S(-68, 64), S(-23, 48), S(-51, 88), S(-47,121), S( -2, 88), S(-58, 67), S(-27, 64),
      S(-12,  3), S( -2, -1), S(-10, 32), S( -9, 57), S( 12, 77), S( 22, 96), S( 39, 68), S(  5, 78),
      S( -1,-19), S(  0, 24), S(-13, 22), S(-16, 73), S(-15, 92), S(-13, 99), S( 21, 89), S( 10, 60),
      S(  6,-36), S(  8, -4), S(  0,  1), S(-11, 48), S( -7, 43), S(  0, 34), S( 14, 15), S( 19, 20),
      S(  3,-61), S( 15,-44), S(  6,-18), S(  2,-26), S(  4,-27), S(  1,-16), S( 22,-48), S( 10,-47),
      S(  4,-69), S( 11,-68), S( 17,-88), S( 12,-43), S( 14,-54), S( 15,-122), S( 13,-113), S( -8,-62),
      S(  2,-80), S( -1,-92), S(  3,-93), S(  7,-82), S(  6,-86), S(  0,-103), S(  1,-84), S( -3,-82) },

    { S(-79,-92), S(-47,-44), S(-68,-13), S(-62,  6), S(-75,-13), S(-66,-25), S(-63,-21), S(-67,-101),
      S(-62,-47), S(-13, 40), S(-31, 42), S(-42, 27), S(-27, 18), S(-31, 32), S(-23, 44), S(-56,-54),
      S(-18, -7), S( 18, 54), S( 27, 65), S( 27, 56), S( 23, 51), S( 39, 61), S( 19, 50), S(-40,-16),
      S(-19, -7), S( 29, 31), S( 36, 68), S( 47, 80), S( 34, 76), S( 57, 56), S( 20, 24), S(-52,-21),
      S(-15,-48), S( 46,  8), S( 67, 43), S( 40, 71), S( 52, 65), S( 56, 37), S( 66, -2), S(-59,-45),
      S(-42,-31), S( 16, -4), S( 31, 20), S( 18, 43), S( 33, 36), S( 15, 24), S( 17, -8), S(-52,-31),
      S( -8,-36), S(-10,-10), S(-22, 10), S(-69, 27), S(-49, 19), S(-42, 15), S( -7,-17), S(-10,-55),
      S(-42,-104), S( -2,-64), S(-33,-31), S(-58,-52), S(-44,-68), S(-83,-25), S( -6,-66), S(-28,-131) },
};

// Initialize the piece square tables with piece values included
CONSTR InitPSQT() {

    // Black scores are negative (white double negated -> positive)
    for (PieceType pt = PAWN; pt <= KING; ++pt)
        for (Square sq = A1; sq <= H8; ++sq) {
            // Base piece value + the piece square value
            PSQT[MakePiece(BLACK, pt)][sq] = -(PieceTypeValue[pt] + PieceSqValue[pt-1][sq]);
            // Same score inverted used for white on the square mirrored horizontally
            PSQT[MakePiece(WHITE, pt)][MirrorSquare(sq)] = -PSQT[pt][sq];
        }
}
