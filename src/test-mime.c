#include "xdgmime.h"
#include "xdgmimeglob.h"
#include <string.h>
#include <stdio.h>


static void
test_individual_glob (const char  *glob,
		      XdgGlobType  expected_type)
{
  XdgGlobType test_type;

  test_type = _xdg_glob_determine_type (glob);
  if (test_type != expected_type)
    {
      printf ("Test Failed: %s is of type %s, but %s is expected\n",
	      glob,
	      ((test_type == XDG_GLOB_LITERAL)?"XDG_GLOB_LITERAL":
	       ((test_type == XDG_GLOB_SIMPLE)?"XDG_GLOB_SIMPLE":"XDG_GLOB_FULL")),
	      ((expected_type == XDG_GLOB_LITERAL)?"XDG_GLOB_LITERAL":
	       ((expected_type == XDG_GLOB_SIMPLE)?"XDG_GLOB_SIMPLE":"XDG_GLOB_COMPLEX")));
    }
}

static void
test_glob_type (void)
{
  test_individual_glob ("*.gif", XDG_GLOB_SIMPLE);
  test_individual_glob ("Foo*.gif", XDG_GLOB_FULL);
  test_individual_glob ("*[4].gif", XDG_GLOB_FULL);
  test_individual_glob ("Makefile", XDG_GLOB_LITERAL);
  test_individual_glob ("sldkfjvlsdf\\\\slkdjf", XDG_GLOB_FULL);
  test_individual_glob ("tree.[ch]", XDG_GLOB_FULL);
}

static void
test_alias (const char *mime_a,
	    const char *mime_b,
	    int         expected)
{
  int actual;

  actual = xdg_mime_mime_type_equal (mime_a, mime_b);

  if (actual != expected)
    {
      printf ("Test Failed: %s is %s to %s\n", 
	      mime_a, actual ? "equal" : "not equal", mime_b);
    }
}

static void
test_aliasing (void)
{
  test_alias ("application/wordperfect", "application/vnd.wordperfect", 1);
  test_alias ("application/x-gnome-app-info", "application/x-desktop", 1);
  test_alias ("application/x-wordperfect", "application/vnd.wordperfect", 1);
  test_alias ("application/x-wordperfect", "audio/x-midi", 0);
  test_alias ("/", "vnd/vnd", 0);
  test_alias ("application/octet-stream", "text/plain", 0);
  test_alias ("text/plain", "text/*", 0);
}

static void
test_subclass (const char *mime_a,
	       const char *mime_b,
	       int         expected)
{
  int actual;

  actual = xdg_mime_mime_type_subclass (mime_a, mime_b);

  if (actual != expected)
    {
      printf ("Test Failed: %s is %s of %s\n", 
	      mime_a, actual ? "subclass" : "not subclass", mime_b);
    }
}

static void
test_subclassing (void)
{
  test_subclass ("application/rtf", "text/plain", 1);
  test_subclass ("message/news", "text/plain", 1);
  test_subclass ("message/news", "message/*", 1);
  test_subclass ("message/news", "text/*", 1);
  test_subclass ("message/news", "application/octet-stream", 1);
  test_subclass ("application/rtf", "application/octet-stream", 1);
  test_subclass ("application/x-gnome-app-info", "text/plain", 1);
  test_subclass ("image/x-djvu", "image/vnd.djvu", 1);
  test_subclass ("image/vnd.djvu", "image/x-djvu", 1);
  test_subclass ("image/vnd.djvu", "text/plain", 0);
  test_subclass ("image/vnd.djvu", "text/*", 0);
  test_subclass ("text/*", "text/plain", 0);
}

int
main (int argc, char *argv[])
{
  const char *result;
  const char *file_name;
  int i;

  test_glob_type ();
  test_aliasing ();
  test_subclassing ();

  for (i = 1; i < argc; i++)
    {
      file_name = argv[i];
      result = xdg_mime_get_mime_type_for_file (file_name);
      printf ("File \"%s\" has a mime-type of %s\n", file_name, result);
    }

#if 1
  xdg_mime_dump ();
#endif
  return 0;
}
     
