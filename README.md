# async_server #

### Install dev dependencies ###

```sh
apt install g++ make
```

### Make ###

```sh
make
```

### Run ###

```sh
make run
```

### Test ###

```
$ curl -w "@curl-format.txt" -o /dev/null -s http://127.0.0.1:8000/

   time_namelookup:  0,000
      time_connect:  0,000
   time_appconnect:  0,000
  time_pretransfer:  0,000
     time_redirect:  0,000
time_starttransfer:  0,000
        ----------
        time_total:  0,000

---------------------------------------------

$ siege -t10s -c 1000 http://localhost:8000/
** SIEGE 3.0.8
** Preparing 1000 concurrent users for battle.
The server is now under siege...
Lifting the server siege..      done.

Transactions:		      108566 hits
Availability:		      100.00 %
Elapsed time:		        9.07 secs
Data transferred:	       17.50 MB
Response time:		        0.07 secs
Transaction rate:	    11969.79 trans/sec
Throughput:		        1.93 MB/sec
Concurrency:		      810.46
Successful transactions:      108566
Failed transactions:	           0
Longest transaction:	        3.08
Shortest transaction:	        0.00

---------------------------------------------

$ ab -n 100000 -c 1000 http://localhost:8000/
This is ApacheBench, Version 2.3 <$Revision: 1706008 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            8000

Document Path:          /
Document Length:        169 bytes

Concurrency Level:      1000
Time taken for tests:   3.582 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      24500000 bytes
HTML transferred:       16900000 bytes
Requests per second:    27919.16 [#/sec] (mean)
Time per request:       35.818 [ms] (mean)
Time per request:       0.036 [ms] (mean, across all concurrent requests)
Transfer rate:          6679.88 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        6   18  74.5     13    1039
Processing:     7   17   4.5     17     217
Waiting:        5   13   4.0     12     214
Total:         19   35  74.9     30    1253

Percentage of the requests served within a certain time (ms)
  50%     30
  66%     33
  75%     35
  80%     36
  90%     38
  95%     41
  98%     44
  99%     45
 100%   1253 (longest request)
```
