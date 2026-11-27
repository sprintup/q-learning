> AI-generated study note — not part of the graded write-up.

# Git submodules (the `src/hw-05` priority queue)

## What a submodule is

A submodule is another git repo living inside this one. Git stores it in two places:

| Where | What it holds |
|---|---|
| `.gitmodules` (a normal tracked file) | The submodule's **path** and **URL** |
| The index / commit tree | A **gitlink**: an entry with mode `160000` that holds the **exact commit** the submodule is pinned to |

The outer repo never stores the submodule's files, only "this folder is repo X at commit Y".

For this project:

| | Value |
|---|---|
| Path | `src/hw-05` |
| URL | `https://github.com/cu-cspb2270-2025/cu-cspb-2270-fall-2026-classroom-hw-05-priority-queue-sprintup` |
| Pinned commit | `4132cdd` |

Both parts are needed. A gitlink without a `.gitmodules` entry can't be restored, because git knows the commit but not where to download it from.

## Restoring it after a clone

A fresh clone leaves `src/hw-05` as an empty folder. Fill it in with either:

```powershell
# Already cloned
git submodule update --init

# Or clone and fetch submodules in one step
git clone --recurse-submodules <q-learning URL>
```

This checks out the **pinned** commit (`4132cdd`), not the newest commit on the
submodule's remote. Inside `src/hw-05` you'll be on a detached HEAD at that commit, which is normal.

If you switch to a branch that pins a different commit, run `git submodule update` again to move the folder to match.

## Inspecting it

```powershell
git submodule status          # pinned commit + path
cat .gitmodules               # path and URL
git ls-tree HEAD src/hw-05    # gitlink in the current commit (mode 160000)
git ls-files -s src/hw-05     # gitlink in the index (staged)
```

The first character of each `git submodule status` line:

| Prefix | Meaning |
|---|---|
| (space) | Checked out at the pinned commit. All good. |
| `-` | Not initialized. Run `git submodule update --init`. |
| `+` | Checked out at a **different** commit than the pin. `git submodule update` resets it; `git add src/hw-05` would make the current commit the new pin. |
| `U` | Merge conflict on the gitlink. |

## Updating the pin to a newer commit

```powershell
git submodule update --remote src/hw-05   # pull the latest commit from the submodule's remote
git add src/hw-05                         # stage the new gitlink
git commit -m "Bump hw-05 submodule"
```

You can also `cd src/hw-05`, `git checkout <commit>`, `cd` back, then `git add src/hw-05` and commit. Either way, the pin only changes when the outer repo commits the new gitlink.

## Adding a submodule

```powershell
git submodule add <URL> src/hw-05
git commit -m "Add hw-05 submodule"
```

`git submodule add` clones the repo, writes `.gitmodules`, and stages both `.gitmodules` and the gitlink.

**Turning an existing clone into a submodule.** If `src/hw-05` is already a clone
(it has its own `.git`), run the same command. Git sees the existing repo and reuses it instead of cloning again:

```powershell
git submodule add <URL> src/hw-05
git commit -m "Add hw-05 submodule"
```

## Lessons from today

- **Plain `git add` on a nested clone is a trap.** `git add src/hw-05` on a folder
  with its own `.git` stages a `160000` gitlink but **no** `.gitmodules` entry. It
  looks like a submodule, but nobody can restore it after cloning. Use
  `git submodule add` instead. (Git prints an "embedded git repository" warning when this happens.)
- **`git clean -ffd` deletes nested repos.** A single `-f` skips folders that
  contain their own `.git`; doubling it (`-ff`) removes them, including any
  unpushed work inside. Preview first with `git clean -ffdn`.
- **Turning a submodule into normal files** (vendoring it):

  ```powershell
  Remove-Item -Recurse -Force src/hw-05/.git   # it's just a folder of files now
  git rm --cached src/hw-05                    # drop the gitlink from the index
  git add src/hw-05                            # add the files as regular files
  ```

  Also delete the `src/hw-05` section from `.gitmodules` (or the whole file if it's
  the only one) and `git add .gitmodules`, then commit.

  In a clone where it's a real submodule, `src/hw-05/.git` may be a file pointing
  into `.git/modules/`, not a folder. `Remove-Item` handles both.
