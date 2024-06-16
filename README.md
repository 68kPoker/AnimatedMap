# Animated Map

## I. Introduction

<b>Animated Map</b> is a graphics engine that draws scrollable map with animated 
elements, like torches, skeletons etc.

## 1. Graphics elements

There are two basic types of animated graphics elements:

- <b>Static GELs</b>  - just animations,
- <b>Dynamic GELs</b> - movable objects.

### a) Drawing and clipping objects
The <b>map</b> and <b>static objects</b> are drawn using fast Blitter mode `D = A`, where 
`A` is a data.

<b>Movable objects</b> are drawn using normal Blitter mode `D = AB + aC`, where 
`A` is a mask, `B` is a data, and `C` is a background.

The movable objects are <b>shifted</b> <cite>in place</cite> by `0` to `15` pixels for easy drawing and tile clipping. It requires just one extra scan-line.

### b) Storing objects

These static objects are stored as `16x16` tiles (one or multiple).

Movable objects are stored as one or multiple `16x16` tiles as well, for better 
graphics organisation. The width and height is then specified in tiles.

The movable object background is stored as tiles. So the multiple object in 
one place are stored once.

### c) Map scrolling and updating

Map (chamber) is scrolled by multiplication of `16` pixels in any direction.

The scrolling is done using tile update. If same tile was already drawn in 
a given place it is not updated there.

Map can be scrolled <b>smoothly</b> using the "shift in place" method like the one mentioned 
above. Once moved by increment greater or equal `16` pixels, the map is updated.

Static objects are then inserted using first/last word mask.

The movable objects are drawn always, when moved or when the tile background 
nearby was affected by static or movable object.

"Animated Map" works in any aligned `Layer/Window/RastPort`. The window borders 
are `16`-pixels thick and can be used to display additional information.

The game is meant to work on Custom Screen due to these display restrictions.

The screen should be double-buffered. Then the previous position in both 
buffers for all movable objects should be stored.

## II. File description
<table>
<tr>
<th>File</th><th>Description</th>
</tr>
<tr>
<th>Screen</th><td>Double buffered screen</td>
</tr>
<tr>
<th>Windows</th><td>Aligned 16x16 windows with borders</td>
</tr>
<tr>
<th>Map</th><td><ol><li>Tile and static object array</li><li>Static object updating</li><li>Map scrolling with optimization</li></ol></td>
</tr>
<tr>
<th>Movable</th><td>
<ol>
<li>Movable object list</li>
<li>Restoring background</li>
<li>Affecting non-changed objects</li>
<li>Comparing the movable object state (position, frame) with previous state in given buffer</li>
</ol</td>
</tr>
<tr>
<th>Blitter</th><td>Low-level drawing algorithms</td>
</tr>
<tr>
<th>Game</th><td>Game-specific routines: 
<ol><li>Static and Movable object animation handlers</li>
<li>Input handlers</li>
</ol></td>
</tr>
</table>