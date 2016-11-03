dyld_cache_extract
==================

[![Build Status](https://img.shields.io/travis/macmade/dyld_cache_extract.svg?branch=master&style=flat)](https://travis-ci.org/macmade/dyld_cache_extract)
[![Issues](http://img.shields.io/github/issues/macmade/dyld_cache_extract.svg?style=flat)](https://github.com/macmade/dyld_cache_extract/issues)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg?style=flat)
![License](https://img.shields.io/badge/license-mit-brightgreen.svg?style=flat)
[![Contact](https://img.shields.io/badge/contact-@macmade-blue.svg?style=flat)](https://twitter.com/macmade)

### About

A macOS command line tool to extract dynamic libraries from the `dyld_shared_cache` of macOS and iOS.

### Usage

    dyld_cache_extract - Extractor utility for DYLD shared cache
    
    Available options:
        
        --help                            Shows this help dialog.
        --info [PATH]                     Displays informations about a dyld_shared_cache file.
        --extract-all [PATH] [OUT_DIR]    Extracts all libraries from a dyld_shared_cache file.
        --extract [LIB] [PATH] [OUT_DIR]  Extracts a specific library from a dyld_shared_cache file.

License
-------

`dyld_cache_extract` is released under the terms of the MIT License.

Repository Infos
----------------

    Owner:			Jean-David Gadina - XS-Labs
    Web:			www.xs-labs.com
    Blog:			www.noxeos.com
    Twitter:		@macmade
    GitHub:			github.com/macmade
    LinkedIn:		ch.linkedin.com/in/macmade/
    StackOverflow:	stackoverflow.com/users/182676/macmade
