# Web Crawler

Web crawler explore the web and save every pages he found.

## Build

### Dependences

- `cmake`
- `ninja` or `make`
- `gcc`
- `sqlite3`
- `curl`

### Build

- **with Ninja :**

```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

- **with Make**

```
mkdir build
cd build
cmake ..
make
```

## User
```
web-crawler <url> --clear-waitlist
