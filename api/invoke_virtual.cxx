/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "api/invoke.h"
#include "api/invoke_virtual.h"
#include "api/java_lang_string.h"
#include "api/java_lang_system.h"
#include "api/amiga.h"
#include "api/draw3d_object.h"
#include "api/draw3d_texture.h"
#include "common/JavaClass.h"
#include "common/JavaCompiler.h"
#include "common/Util.h"

#define DRAW3D "net/mikekohn/java_grinder/draw3d"
#define DRAW3D_LEN (sizeof(DRAW3D) - 1)
#define DRAW3D_TEXTURE "net/mikekohn/java_grinder/draw3d/Texture"
#define DRAW3D_TEXTURE_LEN (sizeof(DRAW3D_TEXTURE) - 1)
#define COPPER "net/mikekohn/java_grinder/amiga/Copper"
#define COPPER_LEN (sizeof(COPPER) - 1)
#define BLITTER "net/mikekohn/java_grinder/amiga/Blitter"
#define BLITTER_LEN (sizeof(BLITTER) - 1)

// FIXME: Is this function ever called?  This looks like it was made to
// deal with stuff like System.out.println() where System is the class,
// out is the field_name / field_type, and println() is the method_name /
// method_sig.
int invoke_virtual(
  JavaClass *java_class,
  int method_id,
  int field_id,
  Generator *generator)
{
  std::string field_name;
  std::string field_type;
  std::string field_class;
  std::string method_name;
  std::string method_sig;
  std::string method_class;
  std::string function;

  //printf("invoke_virtual() (static)\n");

  if (java_class->get_ref_name_type(field_name, field_type, field_id) != 0 ||
      java_class->get_class_name(field_class, field_id) != 0)
  {
    printf("Error: Could not field info for field_id %d\n", field_id);
    return -1;
  }

  if (java_class->get_class_name(method_class, method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

#if 0
  printf("field: '%s as %s' from %s\n",
    field_name.c_str(), field_type.c_str(), field_class.c_str());
  printf("method: '%s as %s' from %s\n",
    method_name.c_str(), method_sig.c_str(), method_class.c_str());
#endif

  function = Util::get_virtual_function(method_name, method_sig, field_name, field_class);

  //printf("function: %s()\n", function);

  int ret = -1;

  if (field_class == "java/lang/System")
  {
    ret = java_lang_system(java_class, generator, function.c_str());
  }

  function = get_static_function(method_name, method_sig);

  printf("function=%s  %s field_class=%s\n",
    function.c_str(), field_name.c_str(), method_class.c_str());

  if (method_class == "java/lang/String")
  {
    ret = java_lang_string(java_class, generator, function.c_str(), field_name.c_str(), field_id);
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s'\n", function.c_str());

  return -1;
}

int invoke_virtual(JavaClass *java_class, int method_id, Generator *generator)
{
  std::string method_name;
  std::string method_sig;
  std::string method_class;
  std::string function;
  bool is_constructor = false;

  //printf("invoke_virtual() (pushed)\n");

  if (java_class->get_class_name(method_class, method_id) != 0 ||
      java_class->get_ref_name_type(method_name, method_sig, method_id) != 0)
  {
    printf("Error: Couldn't get name and type for method_id %d\n", method_id);
    return -1;
  }

#if 0
  printf("method: '%s as %s' from %s\n",
    method_name.c_str(), method_sig.c_str(), method_class.c_str());
#endif

  if (method_name == "<init>")
  {
    method_name = "Constructor";
    is_constructor = true;
  }

  function = get_static_function(method_name, method_sig);

  //printf("virtual function: %s()\n", function.c_str());

  int ret = -1;

#if 0
  if (strcmp(method_class, "java/lang/System") == 0)
  {
    ret = java_lang_system(java_class, generator, function);
  }
#endif

  //printf("function=%s  method_class=%s\n", function.c_str(), method_class.c_str());

  if (method_class == "java/lang/String")
  {
    ret = java_lang_string(java_class, generator, function.c_str());
  }
    else
  if (strncmp(method_class.c_str(), DRAW3D_TEXTURE, DRAW3D_TEXTURE_LEN) == 0)
  {
    const char *cls = method_class.c_str() + DRAW3D_LEN + 1;

    if (is_constructor == true)
    {
      if (method_sig == "(II)V")
      {
        if (strcmp(cls, "Texture16") == 0)
        {
          ret = generator->draw3d_texture_Constructor_II(16);
        }
          else
        if (strcmp(cls, "Texture24") == 0)
        {
          ret = generator->draw3d_texture_Constructor_II(24);
        }
          else
        if (strcmp(cls, "Texture32") == 0)
        {
          ret = generator->draw3d_texture_Constructor_II(32);
        }
          else
        {
          ret = -1;
        }
      }
    }
      else
    {
      do
      {
        ret = -1;

        if (strcmp(cls, "Texture16") == 0)
        {
          ret = draw3d_texture16(java_class, generator, function.c_str());
        }
          else
        if (strcmp(cls, "Texture24") == 0)
        {
          ret = draw3d_texture24(java_class, generator, function.c_str());
        }
          else
        if (strcmp(cls, "Texture32") == 0)
        {
          ret = draw3d_texture32(java_class, generator, function.c_str());
        }

        if (ret != -1) { break; }

        ret = draw3d_texture(java_class, generator, function.c_str());

      } while(0);
    }
  }
    else
  if (strncmp(method_class.c_str(), DRAW3D, DRAW3D_LEN) == 0)
  {
    const char *cls = method_class.c_str() + DRAW3D_LEN + 1;
    int draw3d_type = -1;
    bool with_texture = false;

    if (strcmp(cls, "Points") == 0) { draw3d_type = 0; }
    else if (strcmp(cls, "Line") == 0) { draw3d_type = 1; }
    else if (strcmp(cls, "LineStrip") == 0) { draw3d_type = 2; }
    else if (strcmp(cls, "Triangle") == 0) { draw3d_type = 3; }
    else if (strcmp(cls, "TriangleStrip") == 0) { draw3d_type = 4; }
    else if (strcmp(cls, "TriangleFan") == 0) { draw3d_type = 5; }
    else if (strcmp(cls, "Sprite") == 0) { draw3d_type = 6; }
    else if (strcmp(cls, "TriangleWithTexture") == 0)
    {
      draw3d_type = 3;
      with_texture = true;
    }
    else if (strcmp(cls, "TriangleStripWithTexture") == 0)
    {
      draw3d_type = 4;
      with_texture = true;
    }
    else if (strcmp(cls, "TriangleFanWithTexture") == 0)
    {
      draw3d_type = 5;
      with_texture = true;
    }
    else if (strcmp(cls, "SpriteWithTexture") == 0)
    {
      draw3d_type = 6;
      with_texture = true;
    }

    if (is_constructor == true)
    {
      if (draw3d_type != -1)
      {
        if (method_sig == "(Ljava/lang/String;)V")
        {
          ret = generator->draw3d_object_Constructor_X(draw3d_type, with_texture);
        }
        else if (method_sig == "(I)V")
        {
          ret = generator->draw3d_object_Constructor_I(draw3d_type, with_texture);
        }
      }
    }
      else
    {
      if (strcmp(cls, "Points") == 0 ||
          strcmp(cls, "Line") == 0 ||
          strcmp(cls, "LineStrip") == 0 ||
          strcmp(cls, "Triangle") == 0 ||
          strcmp(cls, "TriangleStrip") == 0 ||
          strcmp(cls, "TriangleFan") == 0 ||
          strcmp(cls, "Sprite") == 0)
      {
        ret = draw3d_object(java_class, generator, function.c_str());
      }
        else
      if (strcmp(cls, "TriangleWithTexture") == 0 ||
          strcmp(cls, "TriangleStripWithTexture") == 0 ||
          strcmp(cls, "TriangleFanWithTexture") == 0 ||
          strcmp(cls, "SpriteWithTexture") == 0)
      {
#if 0
        if (strncmp(function, "setPoint", 8) == 0)
        {
          ret = draw3d_object_with_texture(java_class, generator, function);
        }
          else
        {
          ret = draw3d_object(java_class, generator, function);
        }
#endif
        do
        {
          ret = draw3d_object_with_texture(java_class, generator, function.c_str());
          if (ret == 0) { break; }
          ret = draw3d_object(java_class, generator, function.c_str());
        } while(0);
      }
    }
  }
    else
  if (strncmp(method_class.c_str(), COPPER, COPPER_LEN) == 0)
  {
    if (is_constructor == true)
    {
      if (method_sig == "(I)V")
      {
        ret = generator->copper_Constructor_I();
      }
    }
      else
    {
      ret = copper(java_class, generator, function.c_str());
    }
  }
    else
  if (strncmp(method_class.c_str(), BLITTER, BLITTER_LEN) == 0)
  {
    if (is_constructor == true)
    {
      if (method_sig == "()V")
      {
        ret = generator->blitter_Constructor();
      }
    }
      else
    {
      ret = blitter(java_class, generator, function.c_str());
    }
  }

  if (ret == 0) { return 0; }

  printf("--> Function not implemented '%s'\n", function.c_str());

  return -1;
}

