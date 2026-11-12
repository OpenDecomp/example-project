# OpenDecomp example project

[![Progress](https://dev.opendecomp.com/OpenDecomp/example-project.svg?mode=shield&label=Code)](https://dev.opendecomp.com/OpenDecomp/example-project)
[![Game](https://dev.opendecomp.com/OpenDecomp/example-project.svg?mode=shield&category=game)](https://dev.opendecomp.com/OpenDecomp/example-project)
[![SDK](https://dev.opendecomp.com/OpenDecomp/example-project.svg?mode=shield&category=sdk)](https://dev.opendecomp.com/OpenDecomp/example-project)

A small but complete decompilation project, set up the way [OpenDecomp](https://opendecomp.com)
expects. Use it as the model for your own repository: everything OpenDecomp needs is here,
and nothing else.

The game is made up: a few hundred bytes of our own C code for a made-up PowerPC console, built
with GCC in two versions, `us` and `eu`, which differ a little (frame rate, a damage rule, a
language setting). No real game's code or files are in this repository.

## How OpenDecomp gets your progress

1. Your build makes one [objdiff](https://github.com/encounter/objdiff) progress report per
   game version.
2. A GitHub Actions workflow uploads each report as an artifact named `<version>_report`.
3. OpenDecomp reads those artifacts from successful push runs on your default branch:
   - **Without the GitHub App**, it checks your repository every 30 minutes (every 6 hours
     once a project has had no new run for 30 days).
   - **With the OpenDecomp GitHub App** installed, GitHub tells OpenDecomp the moment a run
     finishes, so new reports show up within a minute or two. The App can also post a
     progress comment on each pull request, comparing the PR's reports with its base commit's.

Your repository needs nothing specific to OpenDecomp: just objdiff's configuration and a CI
workflow that uploads the reports.

## Layout

```
.github/workflows/build.yml   CI: build each version, generate its report, upload it
configure.py                  the unit list; writes build.ninja and objdiff.json
config/<version>/build.sha1   SHA-1 of the original game executable, to check the build
include/                      headers
src/                          the decompiled source, one file per unit
orig/<version>/               your own copy of the game's files (never committed, see below)
tools/link.ld                 linker script
example-only/                 only this example has it (see the end)
```

## Units and what "complete" means

A unit is one object file of the original game. `configure.py` lists every unit linked into
the game, decompiled or not, so the totals cover the whole game. Each unit is in one of these
states:

| State | Example here | In `objdiff.json` |
|---|---|---|
| Linked: matches exactly, and the build links the object built from source | `game/main`, `sdk/os` | `"complete": true` |
| In progress: some functions match, others are missing or not matching yet | `game/player`, `sdk/math` | `"complete": false` |
| No source yet: counted, at 0% | `game/save` | `"base_path": null` |

The build links the game from the linked units' new objects plus the original objects of
every other unit, then checks the result against `config/<version>/build.sha1`. So marking a
unit linked before it really matches fails the build. A unit can be linked in one version and
not in another: `game/player` matches `us` sooner than `eu`, whose damage rule differs.

## The original objects (`orig/`)

Decompilation projects never commit the game's code or data. Each contributor supplies their
own copy of the game in `orig/<version>/`, and the build extracts (splits) the original objects
from it into `build/<version>/obj/`: those are the targets objdiff compares your code against.
CI needs the game files too, so real projects provide them privately, for example in a private
container image the workflow runs in. They must never be in the public repository.

This example's game is made up, so it commits its "original" objects in `orig/<version>/obj/`
(small `.o` files) and its extract step only copies them. Your `.gitignore` should keep
everything in `orig/` out of git.

## The report

`configure.py` writes `objdiff.json` for the chosen version. The parts the report uses:

```json
{
  "units": [
    {
      "name": "game/player",
      "target_path": "build/us/obj/game/player.o",
      "base_path": "build/us/src/game/player.o",
      "metadata": {
        "complete": false,
        "progress_categories": ["game"],
        "source_path": "src/game/player.c"
      }
    }
  ],
  "progress_categories": [
    { "id": "game", "name": "Game" },
    { "id": "sdk", "name": "SDK" }
  ]
}
```

- `progress_categories` declares the categories OpenDecomp shows next to "All". Each unit
  counts toward the categories it lists; they may overlap.
- `complete` marks a linked unit: its code and data count as "linked" on OpenDecomp.
- A unit with `base_path` set to `null` has no source yet; its functions count at 0%.

Then `objdiff-cli report generate -o build/<version>/report.json` compares every unit and
writes the report.

## The workflow

[`.github/workflows/build.yml`](.github/workflows/build.yml) runs on `push` and `pull_request`,
with a matrix job per version. Each job builds, generates the report and uploads it. What
matters to OpenDecomp:

- **Artifact name:** exactly `<version>_report`, for example `us_report`. The version id is the
  name without `_report`. Keep it the same in every run: history is kept per version.
- **Artifact content:** point `actions/upload-artifact` at the single report file, so its zip
  holds `report.json` at the root. Leave `archive` at its default: `archive: false` names the
  artifact after the file instead.
- **Version ids:** letters, digits, `.`, `_` and `-` are safest. No `/`, at most 200 bytes, and
  not ending in a file extension like `.json` or `.svg`.
- **Which runs count:** successful push runs on the default branch become history. Pull request
  runs only feed the PR comment. Runs on other branches are ignored. A project's settings on
  OpenDecomp can also accept failed, scheduled or manually started runs.
- **Pinning:** the workflow downloads objdiff-cli from its GitHub releases at a fixed version
  and checks its SHA-256, and pins each action by commit SHA.

## Put your project on OpenDecomp

1. Push to your default branch and let the workflow finish, so at least one successful run
   has uploaded its `<version>_report` artifacts. GitHub deletes artifacts after their
   retention period (90 days by default), so register while a recent one exists.
2. Sign in at [opendecomp.com/login](https://opendecomp.com/login) with GitHub, open
   [opendecomp.com/manage/new](https://opendecomp.com/manage/new) and enter your repository.
   You need to be an admin of it. Fill in the name, platform and default version.
3. Optional: install the OpenDecomp GitHub App on the repository (your project's management
   page links to it) for instant updates and pull request comments.

## Badges and JSON

Replace `{owner}/{repo}` with your repository, for example `OpenDecomp/example-project`:

| What | URL |
|---|---|
| Badge (default version, matched code) | `https://opendecomp.com/{owner}/{repo}.svg?mode=shield` |
| Badge for a category | `https://opendecomp.com/{owner}/{repo}.svg?mode=shield&category=game` |
| Badge for another version or measure | `https://opendecomp.com/{owner}/{repo}.svg?mode=shield&version=eu&measure=functions` |
| Project data | `https://opendecomp.com/{owner}/{repo}.json` |
| Project data by repository id | `https://opendecomp.com/projects/{id}.json` |

In Markdown:

```markdown
[![Progress](https://opendecomp.com/{owner}/{repo}.svg?mode=shield)](https://opendecomp.com/{owner}/{repo})
```

The repository id (`gh api repos/{owner}/{repo} --jq .id`) never changes, so
`/projects/{id}.json` keeps working after a rename or a transfer. Badges also take `label`,
`style`, `color` and `labelColor`.

This repository is on OpenDecomp's development site, so its badges above come from
`dev.opendecomp.com`: [project page](https://dev.opendecomp.com/OpenDecomp/example-project),
[JSON](https://dev.opendecomp.com/OpenDecomp/example-project.json),
[JSON by id](https://dev.opendecomp.com/projects/1455806835.json).

## Build it yourself

On Ubuntu or Debian (WSL works too):

```sh
sudo apt-get install gcc-powerpc-linux-gnu ninja-build python3
python3 configure.py --version us
ninja
objdiff-cli report generate -o build/us/report.json
```

Get `objdiff-cli`, or the objdiff app, from
[objdiff's releases](https://github.com/encounter/objdiff/releases). Open this folder in the
objdiff app to compare each function with the original as you edit `src/`.

## `example-only/`

Only this example has this folder, because its game is made up: `original/` is the made-up
game's full source, and `make_targets.py` rebuilds `orig/*/obj/` and `config/*/build.sha1`
from it. A real project has neither. If you want to try decompiling the rest yourself, don't
look in there.

This repository is also the test project of OpenDecomp's development site: its pushes and pull
requests exercise the GitHub App's webhook path.

## License

[MIT](LICENSE)
