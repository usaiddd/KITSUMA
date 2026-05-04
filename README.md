# KITSUMA 🗂️
### Hierarchy-Based Version Control System

> A smarter alternative to Git — designed for teams with defined leadership structures and folder-level ownership.

---

## What is KITSUMA?

KITSUMA is a **version control system** built on top of a **PostgreSQL database**, where an Admin sets up a project container, assigns employees to folders, defines a folder head, and establishes a push hierarchy. Instead of everyone pushing to the same place and resolving conflicts repeatedly, KITSUMA batches and chains pushes through the hierarchy — reducing the number of conflict resolutions needed.

**The core idea:**
> In a traditional system, if 3 people push to the same file, conflicts must be resolved 3 separate times. With KITSUMA's hierarchy, they're resolved just once at the top.

---

## Tech Stack

| Layer | Technology |
|---|---|
| CLI / Logic | C++ |
| Database Bridge | Python |
| Database | PostgreSQL (with `pgcrypto`) |

---

## Project Structure

```
KITSUMA/
├── run.cpp          # Entry point — login/signup, container & hierarchy setup
├── add.cpp          # Handles adding/pushing files
├── update.cpp       # Handles pulling/updating files
├── define.cpp       # Defines container structure
├── load.py          # Python bridge between C++ and PostgreSQL
├── test.cpp / test.py  # Testing utilities
├── kitsumadb.sql    # Full database schema
└── not_merged/      # Holds unmerged pushes pending review
```

---

## How It Works

### 1. Admin Signs Up
The Admin creates an account and names a **container** (the project workspace). They set up:
- The **folder/file structure** of the project
- **Employees** (login IDs and passwords auto-created)
- A **folder head** for each root folder
- A **push hierarchy** — the order in which pushes are reviewed and merged

### 2. Employees Log In
Each employee logs in with their assigned credentials and gets access only to their designated folders.

### 3. Pushing Changes
Employees push file changes with a message. Pushes are stored in the database as unmerged, waiting for the hierarchy to process them.

### 4. Hierarchy-Based Merging
Pushes flow up the hierarchy. The folder head reviews and merges them once, rather than resolving conflicts after each individual push — this is KITSUMA's key advantage over Git.

---

## Database Schema (Summary)

| Table | Purpose |
|---|---|
| `users` | Stores login credentials (passwords hashed with bcrypt) |
| `container` | Represents a project workspace |
| `file` | Stores file names and content |
| `pushes` | Tracks every push (content, message, merged status, timestamp) |
| `folder_employees` | Maps employees to their assigned folders |
| `folder_heads` | Designates one head per folder |
| `folder_hierarchy` | Defines the ranked review order within each folder |
| `personalized_files` | Stores each user's local project structure view |

---

## Setup & Installation

### Prerequisites
- C++ compiler (g++)
- Python 3.x
- PostgreSQL (with `pgcrypto` extension)
- Python `psycopg2` library

### Steps

```bash
# 1. Clone the repo
git clone https://github.com/usaiddd/KITSUMA.git
cd KITSUMA

# 2. Set up the database
psql -U postgres -f kitsumadb.sql

# 3. Configure DB connection in load.py
# Edit the connection string at the top of load.py

# 4. Compile the C++ files
g++ run.cpp -o run
g++ add.cpp -o add
g++ update.cpp -o update

# 5. Run KITSUMA
./run
```

---

## Usage Flow

```
./run
  └── Signup (Admin)
        ├── Create container
        ├── Add employees
        ├── Define folder structure
        ├── Assign employees to folders
        ├── Set folder heads
        └── Define push hierarchy

  └── Login (Employee)
        ├── View assigned files
        ├── Push changes  →  ./add
        └── Pull updates  →  ./update
```

---

## Key Concepts

**Container** — A named project workspace. All files, employees, and hierarchies belong to a container.

**Folder Head** — The designated reviewer for a folder. Responsible for merging pushes from their team.

**Hierarchy** — A ranked list of employees per folder. Determines the order in which changes are reviewed and merged upstream.

**Push** — A file change submitted by an employee, stored as unmerged until approved by the folder head.

---

## Why KITSUMA over Git?

| Feature | Git | KITSUMA |
|---|---|---|
| Conflict resolution | After every push | Once, at hierarchy level |
| Access control | Branch-based | Folder & employee-based |
| Role enforcement | Manual/convention | Built into the system |
| Audit trail | Commit history | Push log with timestamps & users |

---

## Contributing

Pull requests are welcome! If you'd like to improve KITSUMA, fork the repo and open a PR. For major changes, open an issue first to discuss what you'd like to change.

---

## Author

**usaiddd** — [github.com/usaiddd](https://github.com/usaiddd)
