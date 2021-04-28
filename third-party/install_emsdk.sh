#!/bin/bash

# Get the emsdk repo
git clone --recursive https://github.com/emscripten-core/emsdk
cd emsdk
git reset --hard e899a6fd5afab39de6d3947d52ed60fa6ed225ad

# Download and install the latest SDK tools.
./emsdk install 2.0.5

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate 2.0.5

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh

# Apply liblzma port patch
# adapted from https://gist.github.com/TrueBrain/fb801387a096352d2f712a70e6a54424
cd upstream/emscripten && patch -p1 << 'EMSDK_LIBLZMA_PATCH_HEREDOC_EOF'
From ba59b660081e366f448ca91e975d4849c2e0b80d Mon Sep 17 00:00:00 2001
From: milek7 <me@milek7.pl>
Date: Tue, 8 Dec 2020 01:03:31 +0100
Subject: [PATCH] Add liblzma port

---
 AUTHORS                                     |   1 +
 ChangeLog.md                                |   1 +
 embuilder.py                                |   3 +
 src/settings.js                             |   3 +
 tests/test_other.py                         |   5 +
 tests/third_party/liblzma/liblzma_test.c    | 283 ++++++++++++++++++++
 tests/third_party/liblzma/liblzma_test.c.xz | Bin 0 -> 3300 bytes
 tools/ports/liblzma.py                      |  56 ++++
 8 files changed, 352 insertions(+)
 create mode 100644 tests/third_party/liblzma/liblzma_test.c
 create mode 100644 tests/third_party/liblzma/liblzma_test.c.xz
 create mode 100644 tools/ports/liblzma.py

diff --git a/embuilder.py b/embuilder.py
index 818262190ed..ab7d5adb7b2 100755
--- a/embuilder.py
+++ b/embuilder.py
@@ -60,6 +60,7 @@
     'harfbuzz',
     'icu',
     'libjpeg',
+    'liblzma',
     'libpng',
     'ogg',
     'regal',
@@ -197,6 +198,8 @@ def main():
       build_port('ogg', libname('libogg'))
     elif what == 'libjpeg':
       build_port('libjpeg', libname('libjpeg'))
+    elif what == 'liblzma':
+      build_port('liblzma', libname('liblzma'))
     elif what == 'libpng':
       build_port('libpng', libname('libpng'))
     elif what == 'sdl2':
diff --git a/src/settings.js b/src/settings.js
index 61cd98939ba..be6fcb678c6 100644
--- a/src/settings.js
+++ b/src/settings.js
@@ -1197,6 +1197,9 @@ var USE_BZIP2 = 0;
 // 1 = use libjpeg from emscripten-ports
 var USE_LIBJPEG = 0;

+// 1 = use liblzma from emscripten-ports
+var USE_LIBLZMA = 0;
+
 // 1 = use libpng from emscripten-ports
 var USE_LIBPNG = 0;

diff --git a/tools/ports/liblzma.py b/tools/ports/liblzma.py
new file mode 100644
index 00000000000..f6ba8cc8ae9
--- /dev/null
+++ b/tools/ports/liblzma.py
@@ -0,0 +1,56 @@
+# Copyright 2020 The Emscripten Authors.  All rights reserved.
+# Emscripten is available under two separate licenses, the MIT license and the
+# University of Illinois/NCSA Open Source License.  Both these licenses can be
+# found in the LICENSE file.
+
+import os
+import shutil
+from tools import building
+
+VERSION = '5.2.5'
+HASH = '7443674247deda2935220fbc4dfc7665e5bb5a260be8ad858c8bd7d7b9f0f868f04ea45e62eb17c0a5e6a2de7c7500ad2d201e2d668c48ca29bd9eea5a73a3ce'
+
+
+def needed(settings):
+  return settings.USE_LIBLZMA
+
+
+def get(ports, settings, shared):
+  ports.fetch_project('liblzma', 'https://tukaani.org/xz/xz-' + VERSION + '.tar.gz', 'xz-' + VERSION, sha512hash=HASH)
+
+  def create():
+    ports.clear_project_build('liblzma')
+
+    source_path = os.path.join(ports.get_dir(), 'liblzma', 'xz-' + VERSION)
+    dest_path = os.path.join(ports.get_build_dir(), 'liblzma')
+
+    shared.try_delete(dest_path)
+    os.makedirs(dest_path)
+    shutil.rmtree(dest_path, ignore_errors=True)
+    shutil.copytree(source_path, dest_path)
+
+    config_args = ['./configure', '--disable-shared']
+
+    if not settings.USE_PTHREADS:
+        config_args.append('--disable-threads')
+
+    building.configure(config_args, cwd=dest_path)
+    building.make(['make', '-j%d' % building.get_num_cores(), '-C' + dest_path])
+
+    ports.install_headers(os.path.join(dest_path, 'src', 'liblzma', 'api'), 'lzma.h')
+    ports.install_headers(os.path.join(dest_path, 'src', 'liblzma', 'api', 'lzma'), '*.h', 'lzma')
+    return os.path.join(dest_path, 'src', 'liblzma', '.libs', 'liblzma.a')
+
+  return [shared.Cache.get('liblzma.a', create, what='port')]
+
+
+def clear(ports, settings, shared):
+  shared.Cache.erase_file('liblzma.a')
+
+
+def process_args(ports):
+  return []
+
+
+def show():
+  return 'liblzma (USE_LIBLZMA=1; public domain)'
EMSDK_LIBLZMA_PATCH_HEREDOC_EOF

cd ../../..
