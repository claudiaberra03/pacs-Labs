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
4. GitHub
5. SSH
8. Q&A

---
## Outline
6. Labs provisional calendar
7. C++ overview
  - essentials
  - tools we'll use
  - tools you should know exist
8. Q&A

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
```
apptainer pull docker://quay.io/pjbaioni/pacs:2025-2026
```

Any time:
```
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

```
apptainer shell --bind /host/dir:/container/dir pacs_2025-2026.sif
```

This makes `/host/dir` available inside the container at `/container/dir`.

---
## PACS container image — Podman

First time:
```
mkdir -p /path/to/shared_files_directory
podman pull --arch=amd64 quay.io/pjbaioni/pacs:2025-2026
```
> We explicitly pull the amd64 image to ensure consistent library paths across platforms (especially Apple Silicon)

Each time:
```
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

```
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
```
rm /path/to/container/image.sif
```

---
### Podman
There are both images and containers.

List containers:
```
podman ps -a -s
```

List images:
```
podman images
```

Remove unused images and stopped containers:
```
podman system prune --all
```

---
## Test the environment
Create a simple test file `test.cpp`
```
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
```
g++ -std=c++20 -Wall -I $mkEigenInc test.cpp -o test.out
./test
```
Remark: if you are not using the container, `mk` variables will not be defined; you should write instead `-I /usr/include/eigen3` or similar

Expected output is
```
Successfully included Eigen.
```


