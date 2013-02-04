luasp
=====

an fork of luasp for making a lua fascgi which could be used by nginx

## Build
install lua5.1 first
install premake4
go to sapi/cgi folder
```
premake4 gmake
make
```

## Usage
copy the luasp_cgi to your bin folder and add following to your (nginx/lighttpd) init script
```
# start cgi
spawn-fcgi -F 4 -u www-data -s /var/run/nginx-fcgi.sock -P /var/run/nginx-fcgi.pid -- /home/cch/mycode/github/luasp/sapi/cgi/bin/luasp_cgi
# stop cgi
cat /var/run/nginx-fcgi.pid | xargs -n 1 kill
```

