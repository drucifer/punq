# punq

A smarter uniq.

## Why?

1) I frequently find myself needing to look for unique lines, or counts of unique lines, in very large files.  The traditional UNIX way to do this is via sort | uniq, or sort -u.  However, this is inconvenient with very large files due to the added overhead of sorting.  Counting is a linear time operation.  Even the very best sort operations are not.  Counting non-consecutive repeated lines may add to the memory overhead of uniq, but it's still far smaller than the memory overhead of sorting, even when there are no repeated lines.  Finally, sort uses temporary files in /tmp/ to handle large files.  I've frequently run into issues with sort filling up /tmp on one of my servers.

2) I was interested in learning the glib API, and this seemed like a good problem to start with.  It's conceptually very simple, but it does require data structures, string handling, and file reading.  My eventual goal is to include threading as well, to handle multiple input files in parallel. (Hence the 'p' in the name...)
