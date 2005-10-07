/* -*- c-file-style: "gnu"; indent-tabs-mode: nil -*- */
/*
 * Ruby Cairo Binding
 *
 * Copyright 2005 Kouhei Sutou <kou@cozmixng.org>
 *
 * This file is made available under the same terms as Ruby
 *
 */

#include "rb_cairo.h"

#define _SELF(self) (RVAL2CRFONTOPTIONS(self))

VALUE rb_cCairo_FontOptions;

static inline void
cr_options_check_status (cairo_font_options_t *options)
{
  rb_cairo_raise_exception (cairo_font_options_status (options));
}

cairo_font_options_t *
rb_cairo_font_options_from_ruby_object (VALUE obj)
{
  cairo_font_options_t *options;
  if (!RTEST (rb_obj_is_kind_of (obj, rb_cCairo_FontOptions)))
    {
      rb_raise (rb_eTypeError, "not a cairo font options");
    }
  Data_Get_Struct (obj, cairo_font_options_t, options);
  return options;
}

static void
cr_options_free (void *ptr)
{
  if (ptr)
    {
      cairo_font_options_destroy ((cairo_font_options_t *) ptr);
    }
}

VALUE
rb_cairo_options_to_ruby_object (cairo_font_options_t *options)
{
  if (options)
    {
      return Data_Wrap_Struct (rb_cCairo_FontOptions, NULL,
                               cr_options_free, options);
    }
  else
    {
      return Qnil;
    }
}

static VALUE
cr_options_allocate (VALUE klass)
{
  return Data_Wrap_Struct (klass, NULL, cr_options_free, NULL);
}

static VALUE
cr_options_create (VALUE self)
{
  cairo_font_options_t *options;

  options = cairo_font_options_create ();
  cr_options_check_status (options);
  DATA_PTR (self) = options;
  return Qnil;
}

static VALUE
cr_options_copy (VALUE self)
{
  cairo_font_options_t *options;

  options = cairo_font_options_copy (_SELF (self));
  cr_options_check_status (options);
  return CRFONTOPTIONS2RVAL (options);
}

static VALUE
cr_options_merge (VALUE self, VALUE other)
{
  cairo_font_options_merge (_SELF (self), _SELF (other));
  return self;
}

static VALUE
cr_options_equal (VALUE self, VALUE other)
{
  return cairo_font_options_equal (_SELF (self), _SELF (other)) ? Qtrue : Qfalse;
}

static VALUE
cr_options_hash (VALUE self)
{
  return INT2NUM (cairo_font_options_hash (_SELF (self)));
}

static VALUE
cr_options_set_antialias (VALUE self, VALUE rb_antialias)
{
  cairo_antialias_t antialias;

  antialias = NUM2INT (rb_antialias);
  
  if (antialias < CAIRO_ANTIALIAS_DEFAULT ||
      antialias > CAIRO_ANTIALIAS_SUBPIXEL)
    {
      rb_raise (rb_eArgError, "invalid antialias");
    }
  
  cairo_font_options_set_antialias (_SELF (self), antialias);
  return self;
}

static VALUE
cr_options_get_antialias (VALUE self)
{
  return INT2NUM (cairo_font_options_get_antialias (_SELF (self)));
}

static VALUE
cr_options_set_subpixel_order (VALUE self, VALUE rb_subpixel_order)
{
  cairo_subpixel_order_t subpixel_order;

  subpixel_order = NUM2INT (rb_subpixel_order);
  
  if (subpixel_order < CAIRO_SUBPIXEL_ORDER_DEFAULT ||
      subpixel_order > CAIRO_SUBPIXEL_ORDER_VBGR)
    {
      rb_raise (rb_eArgError, "invalid subpixel order");
    }
  
  cairo_font_options_set_subpixel_order (_SELF (self), subpixel_order);
  return self;
}

static VALUE
cr_options_get_subpixel_order (VALUE self)
{
  return INT2NUM (cairo_font_options_get_subpixel_order (_SELF (self)));
}

static VALUE
cr_options_set_hint_style (VALUE self, VALUE rb_hint_style)
{
  cairo_hint_style_t hint_style;

  hint_style = NUM2INT (rb_hint_style);
  
  if (hint_style < CAIRO_HINT_STYLE_DEFAULT ||
      hint_style > CAIRO_HINT_STYLE_FULL)
    {
      rb_raise (rb_eArgError, "invalid hint style");
    }
  
  cairo_font_options_set_hint_style (_SELF (self), hint_style);
  return self;
}

static VALUE
cr_options_get_hint_style (VALUE self)
{
  return INT2NUM (cairo_font_options_get_hint_style (_SELF (self)));
}

static VALUE
cr_options_set_hint_metrics (VALUE self, VALUE rb_hint_metrics)
{
  cairo_hint_metrics_t hint_metrics;

  hint_metrics = NUM2INT (rb_hint_metrics);
  
  if (hint_metrics < CAIRO_HINT_METRICS_DEFAULT ||
      hint_metrics > CAIRO_HINT_METRICS_ON)
    {
      rb_raise (rb_eArgError, "invalid hint metrics");
    }
  
  cairo_font_options_set_hint_metrics (_SELF (self), hint_metrics);
  return self;
}

static VALUE
cr_options_get_hint_metrics (VALUE self)
{
  return INT2NUM (cairo_font_options_get_hint_metrics (_SELF (self)));
}


void
Init_cairo_font_options (void)
{
  rb_cCairo_FontOptions =
    rb_define_class_under (rb_mCairo, "FontOptions", rb_cObject);

  rb_define_alloc_func (rb_cCairo_FontOptions, cr_options_allocate);
  
  rb_define_method (rb_cCairo_FontOptions, "initialize", cr_options_create, 0);

  rb_define_method (rb_cCairo_FontOptions, "dup", cr_options_copy, 0);
  rb_define_method (rb_cCairo_FontOptions, "merge", cr_options_merge, 1);
  rb_define_method (rb_cCairo_FontOptions, "eql?", cr_options_equal, 1);
  rb_define_method (rb_cCairo_FontOptions, "hash", cr_options_hash, 0);
  rb_define_method (rb_cCairo_FontOptions, "set_antialias",
                    cr_options_set_antialias, 1);
  rb_define_method (rb_cCairo_FontOptions, "antialias",
                    cr_options_get_antialias, 0);
  rb_define_method (rb_cCairo_FontOptions, "set_subpixel_order",
                    cr_options_set_subpixel_order, 1);
  rb_define_method (rb_cCairo_FontOptions, "subpixel_order",
                    cr_options_get_subpixel_order, 0);
  rb_define_method (rb_cCairo_FontOptions, "set_hint_style",
                    cr_options_set_hint_style, 1);
  rb_define_method (rb_cCairo_FontOptions, "hint_style",
                    cr_options_get_hint_style, 0);
  rb_define_method (rb_cCairo_FontOptions, "set_hint_metrics",
                    cr_options_set_hint_metrics, 1);
  rb_define_method (rb_cCairo_FontOptions, "hint_metrics",
                    cr_options_get_hint_metrics, 0);
}