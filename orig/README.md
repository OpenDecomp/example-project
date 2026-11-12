# orig/

In a real project, each contributor puts their own copy of the game's files here, one folder
per version (`orig/<version>/`), and the build extracts the original objects from them into
`build/<version>/obj/`. Nothing from the game is ever committed: `.gitignore` keeps it out.

This example's game is made up, so `orig/<version>/obj/` holds its "original" objects, compiled
from `example-only/original/`, and the build's extract step only copies them.
