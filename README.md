# Purpose

Create an API to normalize given address into structured hash.

```
# Given address as "780 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA",
$ curl https://localhost/parse?780+Franklin+Ave+Crown+Heights+Brooklyn+NYC+NY+11216+USA

{
  "house_number": "780",
  "road": "franklin ave",
  "suburb": "crown heights",
  "city_district": "brooklyn",
  "city": "nyc",
  "state": "ny",
  "postcode": "11216",
  "country": "usa"
}
```


# Installation

- Build [libpostal](https://github.com/openvenues/libpostal#installation-maclinux)

- Compile `main.cpp` using `g++`

    ```
    g++ main.cpp -lfcgi++ -lfcgi `pkg-config --cflags --libs libpostal` -o address-parser-cgi
    ```
- Start FCGI server using `spawn-fcgi`

    ```
    spawn-fcgi -p 3000 -n address-parser-cgi
    ```
    
- Copy `nginx.conf` to `sites-enabled` directory of NGINX

- Visit `https://localhost/parse?780+Franklin+Ave+Crown+Heights+Brooklyn+NYC+NY+11216+USA` :boom:


# More Installation (For Monitoring purposes)

- Copy generated binary of `g++` to `/usr/bin/`

    ```
    sudo cp address-parser-cgi /usr/bin/address-parser-cgi
    ```

- Copy `init.sh` to `/etc/init.d/`

    ```
    sudo cp init.sh /etc/ini.d/
    ```

- Setup `monit` and copy `monit.conf` to `/etc/monit/conf.d/`

   ```
   sudo cp monit.conf /etc/monit/conf.d/
   ```
