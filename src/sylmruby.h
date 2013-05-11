/*
 * Sylmruby --
 * Copyright (C) 2013 HAYASHI Kentaro
 *
 */

#ifndef __SYLMRUBY_H__
#define __SYLMRUBY_H__

#define SYLMRUBY "sylmruby"
#define SYLMRUBYRC "sylmrubyrc"
#define SYLPHEEDRC "sylpheedrc"

typedef struct _SylMRubyOption SylMRubyOption;

struct _SylMRubyOption {
  /* General section */

  GtkWidget *window;

  /* full path to rcfile */
  gchar *rcpath;
  /* rcfile */
  GKeyFile *rcfile;
};

#endif /* __SYLMRUBY_H__ */
