cmake_minimum_required(VERSION 3.27)
project(colocviu)

set(CMAKE_CXX_STANDARD 17)

add_executable(colocviu main.cpp
        Angajat.h
        Barista.h
        Manager.h
        Ospatar.h
        Tura.h
        Program.h
        Produse.h
        Comenzi.h
        Evenimente.h
        Raport.h
        Import_Export.h)
