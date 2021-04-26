Depicts different aspects of simulation state.
`getters/` take a cell or cardinal coordinate and return particular data from that coordinate (e.g., amount of resource owned by a cell, whether a cell is alive, how many cores are active in a cardinal's cpu, etc.).
`fill_colormaps` translate one piece of data to a color.
`border_colormaps` translate two pieces of data (i.e., either side of a border) to a color.
`renderers/` combine a `getter/` and a color map to draw one aspect of a visualization (cell fill, cardinal fill, or borders between cell).
`artists/` layer several `renderers/` to create a complete visualization.
This code is used both to draw on JavaScript `Canvas`es in the web viewer and to generate native png output.
