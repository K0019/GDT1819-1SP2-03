#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* File: Definitions.h
   Use: Misc. constants used in this program that can be changed easily through modifying these values */

constexpr auto PROGRAM_WIDTH = 800;
constexpr auto PROGRAM_HEIGHT = 600;

// All _FIDELITY and _HORIZONTAL_FIDELITY definitions are the number of vertices per splice
// All _VERTICAL_FIDELITY definitions are the number of splices per mesh

// Power saving: 72, Max: 360
#define CIRCLE_FIDELITY 360 / 72

// Power saving: 72, Max: 360
#define RING_FIDELITY 360 / 72

// Power saving: 72, Max: 360
#define SPHERE_HORIZONTAL_FIDELITY 360 / 72
// Power saving: 36, Max 180
#define SPHERE_VERTICAL_FIDELITY 180 / 36

// Power saving: 36, Max 180
#define QUARTERSPHERE_HORIZONTAL_FIDELITY 180 / 36
// Power saving: 18, Max 90
#define QUARTERSPHERE_VERTICAL_FIDELITY 90 / 18

// Power saving: 72, Max 360
#define CYLINDER_FIDELITY 360 / 72

// Lights initialized within the scene
#define NO_OF_POINTLIGHTS 0
#define NO_OF_SPOTLIGHTS 2

#endif // !DEFINITIONS_H
