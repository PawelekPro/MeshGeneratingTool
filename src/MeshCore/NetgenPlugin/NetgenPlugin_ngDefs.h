/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NetgenPlugin_ngDefs_H
#define NetgenPlugin_ngDefs_H

#ifdef WIN32

#if defined NETGENPLUGIN_EXPORTS || defined NETGENEngine_EXPORTS
#define NETGENPLUGIN_EXPORT __declspec(dllexport)
#else
#define NETGENPLUGIN_EXPORT __declspec(dllimport)
#endif

#if defined(NETGEN_V5) || defined(NETGEN_V6)
#define NETGENPLUGIN_DLL_HEADER DLL_HEADER
#else
#define NETGENPLUGIN_DLL_HEADER
#endif

#else

#define NETGENPLUGIN_EXPORT
#define NETGENPLUGIN_DLL_HEADER

#endif

#endif