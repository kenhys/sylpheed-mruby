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

static void exec_sylmruby_menu_cb(void);
static GtkWidget *create_config_main_page(GtkWidget *notebook, GKeyFile *pkey);
static GtkWidget *create_preference_dialog(SylMRubyOption *option);
static void save_sylmruby_preference(SylMRubyOption *option);

#endif /* __SYLMRUBY_H__ */
