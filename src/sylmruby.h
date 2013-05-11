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

/* callback prototypes */
static void init_done_cb(GObject *obj, gpointer data);
static void app_exit_cb(GObject *obj, gpointer data);

static void folderview_menu_popup_cb(GObject *obj, GtkItemFactory *ifactory,
                                     gpointer data);
static void summaryview_menu_popup_cb(GObject *obj, GtkItemFactory *ifactory,
                                      gpointer data);

static void textview_menu_popup_cb(GObject *obj, GtkMenu *menu,
                                   GtkTextView *textview,
                                   const gchar *uri,
                                   const gchar *selected_text,
                                   MsgInfo *msginfo);

static void menu_selected_cb(void);

static void compose_created_cb(GObject *obj, gpointer compose);
static void compose_destroy_cb(GObject *obj, gpointer compose);
static gboolean compose_send_cb(GObject *obj, gpointer compose,
                                gint compose_mode, gint send_mode,
                                const gchar *msg_file, GSList *to_list);
static void messageview_show_cb(GObject *obj, gpointer msgview,
                                MsgInfo *msginfo, gboolean all_headers);
static void inc_start_cb(GObject *obj, PrefsAccount *ac);
static void inc_finished_cb(GObject *obj, gint new_messages);

#endif /* __SYLMRUBY_H__ */
