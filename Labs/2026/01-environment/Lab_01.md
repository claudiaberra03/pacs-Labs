---
marp: true
size: 16:9
style: |
    img[alt~="center"] {
      display: block;
      margin: 0 auto;
    }
---

# Laboratory 01
## Local Development Environment

### Paolo Joseph Baioni
### 27/02/2026

---
## Outline
1. Labs Environment
2. Bash
3. Compilation
4. Git & GitHub
5. SSH
6. Q&A

<!-- ---
## Outline
6. Labs provisional calendar
7. C++ overview
  - essentials
  - tools we'll use
  - tools you should know exist
8. Q&A -->

---
## 1. Labs Environment

We will rely on two environments:
- a local one, running on your PC
- a remote one, running on a dedicated HPC cluster

For the local one, you will need:

1. a text editor (VSCode/Codium, gedit, nano, emacs, vim...)

---
2. a Unix/Linux-based development environment, such as:
   + any Linux distribution (e.g. Debian/Ubuntu, RHEL/Fedora, Arch...; see e.g. [Linux Essentials book](https://www.bibliosearch.polimi.it/permalink/39PMI_INST/1nvvje2/alma9956393108776), available online at PoliMi library website, for a first introduction)
   + Windows + WSL (official WSL installation guide [here](https://learn.microsoft.com/en-us/windows/wsl/install); if in doubt, stick to defaults)
   + macOS

If you have to install Linux for the first time (WSL or native), Ubuntu 24.04 LTS is recommended for compatibility with the course material.

---
## Containers

In all environments, while not strictly required, it is advisable to use containers for isolation, portability, and reproducibility.

We suggest:

- Linux → [Apptainer](https://apptainer.org/docs/user/main/quick_start.html)
- WSL → [Apptainer](https://apptainer.org/docs/user/main/quick_start.html)
- macOS → [Docker](https://www.docker.com/get-started/) or [Podman](https://podman.io/get-started)

The HPC cluster runs Linux and already has Apptainer installed.

---
## Container model

Image → blueprint  
Container → running instance

An image is immutable.  
A container is a runtime instance of an image.

---
## PACS container image — Apptainer

First time:
```bash
apptainer pull docker://quay.io/pjbaioni/pacs:2025-2026
```

Any time:
```bash
apptainer shell pacs_2025-2026.sif
```

(`Ctrl + D` or `exit` to leave)

---
### Remark — File System

Apptainer automatically mounts some host file systems into the container.

Normally, `shell` is what you want:  
your personal data in `/home/username` is available,  
while container libraries and binaries are used.

If you need to override defaults, use `--bind`:

```bash
apptainer shell --bind /host/dir:/container/dir pacs_2025-2026.sif
```

This makes `/host/dir` available inside the container at `/container/dir`.

---
## PACS container image — Podman

First time:
```bash
mkdir -p /path/to/shared_files_directory
podman pull --arch=amd64 quay.io/pjbaioni/pacs:2025-2026
```
> We explicitly pull the amd64 image to ensure consistent library paths across platforms (especially Apple Silicon)

Each time:
```bash
podman run -it --name pacs2526 --rm -v \
/path/to/shared_files_directory:/home/ubuntu/shared_files \
quay.io/pjbaioni/pacs:2025-2026
```

---
### Remarks

- Podman isolates the file system by default.  
  For this reason, it is useful to create an ad-hoc shared directory: inside the container you will only see that host directory.

- `--rm` automatically removes the container (not the image) when it stops -- apptainer-like behaviour.

If you want persistent changes inside the container, omit `--rm`:

```bash
podman run -it --name pacs2526 -v \
/path/to/shared_files_directory:/home/ubuntu/shared_files \
quay.io/pjbaioni/pacs:2025-2026
exit
podman start -ai pacs2526
```

---
## Listing and cleaning

### Apptainer
The only non-temporary files are the container images (usually `.sif` files).

To remove one:
```bash
rm /path/to/container/image.sif
```

---
### Podman
There are both images and containers.

List containers:
```bash
podman ps -a -s
```

List images:
```bash
podman images
```

Remove unused images and stopped containers:
```bash
podman system prune --all
```

---
## Test the environment
Create a simple test file `test.cpp`
```cpp
#include <Eigen/Dense>
#include <iostream>

int main(int argc, char** argv)
{
  std::cout << "Successfully included Eigen." << std::endl;
  return 0;
}
```

---
## Test the environment
Compile and run
```bash
g++ -std=c++20 -Wall -I ${mkEigenInc} test.cpp -o test.out
./test
```
Remark: if you are not using the container, `mk` variables will not be defined; you should write instead `-I /usr/include/eigen3` or similar

Expected output is
```bash
Successfully included Eigen.
```

---
# 2. Bash
<style scoped>
{
  font-size: 25px;
}
</style>
*"A shell is a program that provides the traditional, **text-only** user interface for Linux and other Unix-like operating systems. Its primary function is to read commands that are typed into a console [...] and then execute (i.e., run) them. The term shell derives its name from the fact that it is an outer layer of an operating system. A shell is an **interface** between the **user** and the internal parts of the **OS** (at the very core of which is the kernel)."*
![bg right width:600](./files/shell.png)

From [http://www.linfo.org/shell.html](http://www.linfo.org/shell.html)

---
### What is `Bash`?
- It stands for `Bourne Again Shell`, a homage to its creator Stephen Bourne. 
- It is the default shell for most Linux distributions.
- It is both a *command interpreter* and a *scripting language*.

**Remark:** macOS has replaced it with [zsh](https://support.apple.com/en-us/HT208050), which is mostly compatible with `Bash`, since v10.15 Catalina; modern BSD and a few Linux distributions run zsh as well.

---
## Common bash commands
We will learn Bash through examples during the labs. In the meantime, it is useful to know

### Navigation
```bash
pwd        # where am I?
ls         # list files
cd dir     # enter directory
cd ..      # go up
```

---
### File management

```bash
mkdir dir      # create directory
cp a b         # copy
mv a b         # move/rename
rm file        # remove file
```
---
### Viewing files

```bash
cat file
less file
```

### Help

```bash
man command
command --help
```

---
### Wildcards

```bash
*.cpp: seleziona tuti i file che terminano con l'estensione .cpp
*.txt: seleziona tutti i file di testo
```

### History

```bash
history
↑ arrow
```
---
### Tab completion
```bash
TAB: se premuto mentre si sta scrivendo il nome di un file o di una cartella, Bash cercherà
     di indovinare cosa si vuole scrivere e completerà il nome
```
Example

```bash
cd Do<TAB>
```

becomes

```bash
cd Documents/ (se "Documents" è l'unica cartella che inizia con "Do")
```


You can refer to [https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md) for more, optional, insights.

---
# 3. Compilation and linking
All in one stage
```bash
g++ files/Hello.cc files/World.cc files/main.cc
```
Compilation
```bash
g++ -c files/World.cc
g++ -c files/Hello.cc 
g++ -c files/main.cc 
```
Linking
```bash
g++ Hello.o World.o main.o
```

---
# 4. Git & GitHub
`git` (Linus Torvalds, 2005) is a FOSS distributed version control system.

Version control is the practice of tracking and managing changes to software code. Version control systems are software tools that help software teams manage changes to source code over time.

GitHub is a hosting service for remote git repositories.

---
## Essential Workflow

![bg right:45% width:580](./files/GitDiagram.svg)
<style scoped>
{
  font-size: 24px;
}
</style>
1. Create (or find) a repository with a git hosting tool (an online platform that hosts you project, like GitHub or GitLab)
2. `clone` (download) the repository 
3. `add` a file to your local repo 
4. `commit` (save) the changes, this is a local action, the remote repository (the one in the cloud) is still unchanged
5. `push` your changes, this action synchronizes your version with the one in the hosting platform

---
## Other must-have commands
- `git status` list the status of all the files (e.g. which files have been changed, which are new, which are deleted and which have been added)
- `git diff` show the differences between your code and the last commit
- `log` show the history of commits


---
## Collaborating
If you and your teammates work on different files the workflow is the same as before, you just have to remember to `pull` the changes that your colleagues made.

If you have to work on the same files, the best practice is to create a new `branch`, which is a particular version of the code that branches from the main one. After you have finished working on your feature you `merge` the branch into the main.

To switch among branches you can use `checkout`.

---
## Configuring `git` locally
Configure your machine, using the same email you'll use as primary address on GitHub
```bash
git config --global \
  user.name "Name Surname"
git config --global \
  user.email "name.surname@email.com"
```

---
## More options
You can also edit manually the config file `~/.gitconfig`, as well as specify some other useful options, such as
```ini
[user]
	name = myname
	email = name.surname@email.com
[init]
	defaultBranch = main
[core]
	editor = nano -liST 2
```

---
# 5. SSH authentication
![bg right:45% width:500px](./files/rsa.png)
1. Generate keys with
```bash
ssh-keygen -t ed25519
```
2. Copy the public key
```bash
cat ~/.ssh/id_ed25519.pub
```
3. Add it to the remote host authorized keys [(GitHub)](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)

---
## Exercise
1. Create a private repo on GitHub
2. Clone it locally with ssh protocol (`git clone git@github.com:my_username/my_reponame.git`)
3. Do a change, then
 - `status`
 - `diff`
 - `commit`
 - `push`
4. Do another change directly from the GitHub WebUI
5. Do `git pull` locally

---
## HPC Cluster Access
During the next lab (06/03) we'll use an HPC cluster.

To do so, you'll have to compile a Form (accessible only via PoliMi SSO -- check WeBeep announcements), with which you'll ask for a cluster account and VPN access.

---
## A note on eduroam
We strongly suggest using eduroam with TTLS protocol (PoliMi credentials -- w/o certificate). It is known that, for Linux system, the provided script might fail. You can anyway download it, and inspect it in order to set the connection manually, for example:
1. find your interface name (we'll assume wlo1)
```bash
$ ip a | grep UP | grep w
3: wlo1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
```
2. eliminate previous failed connections
```bash
$ nmcli -t -f NAME connection show | grep -i eduroam | while read c; do
  nmcli connection delete "$c"
done
```

3. add new `eduroam` connection
```bash
$ nmcli connection add \
type wifi \
ifname wlo1 \
con-name eduroam \
ssid eduroam
```

4. configure it
```bash
nmcli connection modify eduroam \
wifi-sec.key-mgmt wpa-eap \
802-1x.eap ttls \
802-1x.identity "your_person_code@polimi.it" \
802-1x.anonymous-identity "anonymous@polimi.it" \
802-1x.phase2-auth pap
```

5. activate it, asking for password prompt
```bash
$ nmcli connection up eduroam --ask
```


