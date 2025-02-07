This tool converts CMPack'02 (and possibly later) motion files to the
Tekkotsu format.
     http://www.tekkotsu.org/dox/classMotionSequence.html

The included motion files are little endian - the format the Aibo and x86
machines use.  However, you will not be able to directly convert these
files when running on a different architecture (i.e. Mac)

If you do not have access to an x86, you can obtain the CMPack'02 release,
futz with the genmot code (agent/Motion/genmot) and have it recreate the
motion files for your architecture.

