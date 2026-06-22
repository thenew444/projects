# GTK4 Architecture Reference

A layered breakdown of GTK4 — what each layer is, what it gives you, and how
it connects to the layers above and below it. Read top to bottom for the full
picture, or jump to a section.

---

## 0. The Big Picture First

GTK4 is not one library — it's a stack of four libraries, each building on
the last:

```
Your application code
        |
   GTK4            <- widgets, application lifecycle, styling, layout
        |
   Gdk / GdkPixbuf  <- windowing system abstraction, input events, images
        |
   GObject          <- type system: properties, signals, inheritance
        |
   GLib             <- base utilities: lists, strings, the event loop
```

Everything you call is ultimately a GObject method underneath, dressed up
with GTK-specific naming. Once you see *that*, the "thousands of functions"
stop looking like thousands of unrelated things and start looking like a
few patterns, repeated.

**The four patterns**, which account for the overwhelming majority of every
GTK function you will ever call:

1. **Construct**: `gtk_button_new()`, `gtk_box_new()` — makes an object
2. **Get/Set**: `gtk_widget_set_visible()` / `gtk_widget_get_visible()` —
   reads or writes a property
3. **Do**: `gtk_window_present()`, `gtk_box_append()` — performs an action,
   not tied to a property
4. **Connect**: `g_signal_connect(widget, "clicked", callback, data)` —
   hooks a function to an event

Keep this in your head as you read everything below — it's the skeleton
that every layer hangs off of.

---

## 1. GLib — The Floor

GLib is not GUI-specific at all. It's C's "missing standard library":

- **Data structures**: `GList`, `GHashTable`, `GArray`, `GString`
- **The main loop**: `GMainLoop` / `GMainContext` — the event loop that
  makes the whole application responsive. When you call
  `g_application_run()`, you are starting this loop. Every signal,
  every redraw, every input event is dispatched through it.
- **Memory & utility**: `g_malloc`, `g_strdup`, timers, threads

**Why it matters to you:** you'll touch this mostly indirectly. But
`GMainLoop` is *why* GTK programs are event-driven rather than
request/response — your `main()` function doesn't run your logic, it just
starts the loop and your logic runs later, triggered by signals.

---

## 2. GObject — The Type System

This is the layer that makes everything else possible, and it's worth
understanding properly rather than glossing over, because it explains *why*
GTK's API looks the way it does.

### Properties

Every GObject-derived class can declare **properties** — named, typed
values with optional getter/setter semantics, *plus* automatic change
notification. This is different from a plain C struct field:

```c
g_object_set(widget, "visible", FALSE, NULL);
g_object_get(widget, "visible", &is_visible, NULL);
```

GTK widgets wrap nearly every property in a dedicated function
(`gtk_widget_set_visible()`), but the generic form above *always* works,
even for properties that don't have a dedicated wrapper. This is also
exactly the mechanism Blueprint/GtkBuilder XML uses under the hood — when
you write `visible: false;` in a `.blp` file, it compiles down to a
property set, not a function call.

### Signals

A **signal** is a named event a class can emit. `GtkButton` defines a
`"clicked"` signal; you attach a callback with:

```c
g_signal_connect(button, "clicked", G_CALLBACK(on_clicked), user_data);
```

Signals are GObject's mechanism, but the *meaning* of each signal
(`"clicked"`, `"activate"`, `"notify::property-name"`) is defined by GTK.
`"notify::visible"` is a generic signal GObject provides for *every*
property automatically — fires whenever that property changes, regardless
of who changed it.

### The type system & casting macros

GObject implements single inheritance + interfaces in plain C, using
macros like:

```c
GTK_WIDGET(ptr)      // cast + runtime type-check
GTK_IS_BUTTON(ptr)   // type-check without casting
```

These exist because C has no native object model — GObject is simulating
one. Every GTK class fits into a hierarchy rooted at `GObject`, e.g.
`GObject -> GtkWidget -> GtkButton`.

### Reference counting

GObjects are reference-counted, not garbage collected:

```c
g_object_ref(obj);    // increment refcount
g_object_unref(obj);  // decrement; frees at 0
```

Widgets that are part of a container tree are usually owned by their
parent, so you often don't manually manage their refcounts — but
standalone objects (a `GtkBuilder`, a list model you created yourself)
need explicit `unref` when you're done.

### Interfaces

GObject also supports interfaces (like Java/C# interfaces) — a class can
implement multiple of these even though it can only inherit from one
parent class. GTK uses this for cross-cutting capabilities:

- **GtkOrientable** — "has a horizontal/vertical orientation"
  (GtkBox, GtkPaned, GtkSeparator all implement this)
- **GtkEditable** — "is a text-editing widget" (shared by entry-like
  widgets so they can share editing logic)
- **GtkAccessible** — exposes semantic info to screen readers
- **GListModel** — "represents an ordered list of items" (huge in GTK4's
  list/grid view system — see Section 6)

**Tie-back:** this is the layer that explains why `gtk_orientable_set_orientation()`
exists as a *separate* function family from `gtk_box_*` — orientation isn't
a GtkBox-specific concept, it's an interface multiple unrelated widgets
share.

---

## 3. Gdk — The Platform Abstraction

Gdk sits between GObject and GTK widgets, abstracting away whether you're
running on X11, Wayland, Windows, or macOS.

- **GdkDisplay** — a connection to a windowing system
- **GdkSurface** — a native window/surface (what a GtkWindow ultimately
  draws into)
- **GdkEvent** — input events: key presses, pointer motion, touch
- **GdkClipboard**, **GdkDrag** — clipboard and drag-and-drop
- **GdkTexture / GdkPaintable** — GTK4's modern image/drawable abstraction,
  replacing most uses of the older GdkPixbuf

**Tie-back:** when a key is pressed, Gdk captures the native OS event,
wraps it in a `GdkEvent`, and GTK's widget tree dispatches it — eventually
firing a signal like `"key-pressed"` on whichever widget has focus. You
rarely call Gdk functions directly unless you're doing custom input
handling, multi-monitor logic, or clipboard/drag-and-drop work.

---

## 4. GtkWidget — The Universal Base

Every visible GTK4 element inherits from `GtkWidget`, which is why its API
surface is the highest-leverage thing to know — everything below this
section is layered on top of it.

**Layout properties** (read by whatever `GtkLayoutManager` the parent
uses — see Section 9):
- `halign` / `valign` — how the widget positions itself within its
  allocated space
- `hexpand` / `vexpand` — whether the widget wants extra space if available
- `margin-start/end/top/bottom`
- size request (`set_size_request`) — a minimum size hint

**State properties:**
- `visible` — shown/hidden
- `sensitive` — enabled/disabled (grayed out + non-interactive when false)
- `can-focus` / `focusable` — whether it participates in keyboard focus
- `opacity`

**Structure (GTK4's child-iteration model):**

GTK3 had `gtk_container_get_children()` returning a list. GTK4 removed the
generic `GtkContainer` class entirely — there's no single "container" base
class anymore. Instead, every widget (container or not) exposes its
children via direct tree navigation:

```c
GtkWidget *child = gtk_widget_get_first_child(widget);
while (child != NULL) {
  // do something with child
  child = gtk_widget_get_next_sibling(child);
}
```

This is *the* biggest structural change from GTK3 — "container" is no
longer a distinct class, just a description of *role*. Any widget can have
children if it chooses to allow them.

**Styling hooks:**
- `gtk_widget_add_css_class()` / `remove_css_class()` — attach CSS class
  names that a `GtkCssProvider` (Section 8) can target
- `gtk_widget_set_name()` — a CSS node identifier (not a visible label)

**Tie-back:** when you call `gtk_box_append(box, child)`, internally GTK
is inserting `child` into `box`'s widget tree (so `get_first_child` /
`get_next_sibling` will find it) *and* registering it with box's layout
manager (`GtkBoxLayout`) so it participates in the linear layout
algorithm. Two different layers — generic widget tree + parent's chosen
layout manager — cooperating.

---

## 5. Widget Categories — Where Most "Elements" Live

These are all `GtkWidget` subclasses; the API for any of them includes
everything from Section 4 *plus* their own specific properties/methods.

| Category | Examples | Role |
|---|---|---|
| Layout containers | `GtkBox`, `GtkGrid`, `GtkStack`, `GtkPaned`, `GtkCenterBox`, `GtkOverlay` | Arrange other widgets |
| Display | `GtkLabel`, `GtkImage`, `GtkPicture`, `GtkSpinner`, `GtkProgressBar` | Show static or status content |
| Input | `GtkEntry`, `GtkSpinButton`, `GtkSwitch`, `GtkScale`, `GtkCheckButton`, `GtkColorButton` | Collect user input |
| Buttons | `GtkButton`, `GtkToggleButton`, `GtkMenuButton`, `GtkLinkButton` | Trigger actions |
| List/data display | `GtkListView`, `GtkColumnView`, `GtkGridView` | Render a `GListModel` (Section 6) |
| Windows & dialogs | `GtkWindow`, `GtkApplicationWindow`, `GtkDialog`, `GtkAboutDialog`, `GtkFileDialog` | Top-level surfaces |
| Menus & popovers | `GtkPopoverMenu`, `GtkPopover` | Contextual floating UI, often built from a `GMenuModel` |

Most of these widgets follow predictable naming conventions once you know
them: boolean-toggle widgets use `_set_active()`/`_get_active()`; range
widgets use `_set_value()`/`_get_value()`; text-holding widgets use
`_set_text()` (entries) or `_set_label()` (buttons/labels) inconsistently
— this naming drift is mostly historical, not principled.

---

## 6. The Model Layer — GTK4's Biggest Conceptual Shift from GTK3

GTK3 had `GtkTreeModel` and `GtkListStore` tightly coupled to
`GtkTreeView`. GTK4 generalized this into something reusable:

- **`GListModel`** — a GObject *interface*: "I am an ordered, possibly
  changing, list of objects." Nothing more. It doesn't know how to render
  itself.
- **Concrete model implementations**: `GListStore` (a plain mutable list
  you populate yourself), `GtkStringList` (specialized for strings)
- **Model transformers** — wrap one model to change what it presents,
  without touching the underlying data:
  - `GtkFilterListModel` — shows a filtered subset
  - `GtkSortListModel` — shows a sorted view
  - `GtkSelectionModel` (interface, e.g. `GtkSingleSelection`,
    `GtkMultiSelection`) — tracks which items are selected

- **Rendering**: `GtkListView`, `GtkColumnView`, `GtkGridView` take *any*
  `GListModel` plus a `GtkListItemFactory` (which tells it how to build a
  widget for each row/item) and handle virtualization (only rendering
  visible rows) automatically.

**Tie-back:** this is the same "separate data from presentation"
philosophy as Section 9's layout managers — GTK4 consistently pulled
*behavior* out of widget subclasses and into composable, swappable objects
the widget just *uses*. A `GtkColumnView` doesn't know or care if its data
came from a database query wrapped in a custom `GListModel`, a
`GtkFilterListModel` narrowing another model, or a plain `GListStore` you
filled by hand.

---

## 7. Application Architecture

- **`GtkApplication`** (a `GApplication` subclass) — manages startup,
  single-instance enforcement (so launching your app twice activates the
  existing instance instead of opening two), and ties into the action
  system below. You call `g_application_run()` on it, which starts the
  GLib main loop from Section 1.
- **`GAction` / `GActionMap` / `GMenuModel`** — a layer of indirection
  between "the user did something" and "code that runs." Instead of
  wiring a button's `"clicked"` signal directly to a function, you can
  define a named action (`"win.save"`) once, and have menu items,
  buttons, *and* keyboard shortcuts all trigger it — without each needing
  its own signal connection.
- **`GtkBuilder`** — loads a widget tree from XML (`.ui` files) at
  runtime, looking up objects by ID afterward
  (`gtk_builder_get_object(builder, "submit_button")`).

**Tie-back to Blueprint:** Blueprint never touches your running program —
it's a compile-time step. `.blp` → `blueprint-compiler` → `.ui` XML →
loaded by `GtkBuilder` at runtime, exactly the same as if you'd
hand-written the XML. Blueprint replaces *writing the XML*, not
`GtkBuilder` itself.

---

## 8. Styling & Rendering

- **`GtkCssProvider`** — loads CSS rules and applies them to widgets,
  matched via the CSS classes you attached in Section 4
  (`add_css_class()`). GTK4 widgets are styled almost entirely through
  CSS now, far more than GTK3's StyleContext-heavy approach.
- **`GtkSnapshot`** — the modern drawing API. If you're writing a custom
  widget that needs to draw itself (rather than composing existing
  widgets), you override its size/draw behavior and issue drawing
  commands to a `GtkSnapshot`, which GTK turns into a render tree
  efficiently. This replaced most direct Cairo usage from GTK3.
- **`GtkStyleContext`** — still exists, but most of what it used to do
  (look up colors, fonts) has been simplified; CSS classes + properties
  are now the primary styling interface.

---

## 9. Layout Managers — Why "Containers" Aren't a Class Anymore

This is the architectural answer to "what even *is* a container in GTK4,"
and ties directly back to Section 4's removal of `GtkContainer`.

- **`GtkLayoutManager`** (base class) — encapsulates *only* the
  arrangement algorithm: given a widget's children and available space,
  where does each child go?
- Concrete implementations: `GtkBoxLayout` (linear), `GtkGridLayout`
  (rows/columns), `GtkBinLayout` (single child, no extra positioning
  logic), `GtkOverlayLayout` (stacked children)

A widget like `GtkBox` is, architecturally, just: a `GtkWidget` + a
`GtkBoxLayout` assigned as its layout manager + some convenience methods
(`gtk_box_append`) for adding children to its tree. The "linear arrangement"
behavior isn't baked into a `GtkBox`-specific subclass hierarchy — it's a
pluggable object `GtkBox` happens to use by default.

**Why this matters practically:** if you're writing a custom widget that
needs box-like arrangement of *its* children, you don't need to subclass
`GtkBox` — you can give your custom `GtkWidget` subclass a `GtkBoxLayout`
directly via `gtk_widget_class_set_layout_manager_type()`, and get the
same arrangement logic without inheriting any of GtkBox's other baggage.

---

## How It All Connects — A Single Walkthrough

Tying every layer together through one concrete action: **the user clicks
a button in a `GtkBox` inside a `GtkApplicationWindow`.**

1. **Gdk** captures the native OS mouse-click event on the window's
   `GdkSurface`, wraps it as a `GdkEvent`.
2. GTK's input-handling dispatches the event up through the **widget
   tree** (Section 4) to determine which widget was clicked — using the
   same parent/child structure that `get_first_child`/`get_next_sibling`
   expose.
3. `GtkButton` recognizes this as a "press and release while still
   inside the button" and emits its `"clicked"` **signal** — a GObject
   mechanism (Section 2), but a meaning GTK defined.
4. Your `g_signal_connect()`'d callback runs, synchronously, inside the
   **GLib main loop** (Section 1) that's been running since
   `g_application_run()`.
5. Suppose your callback calls `gtk_widget_set_sensitive(other_widget, FALSE)`
   — this is a **property set** (Section 2's generic mechanism, wrapped
   in a GTK-specific function per Section 4), which also fires a
   `"notify::sensitive"` signal automatically, in case anything else is
   listening.
6. That property change invalidates layout/rendering for that widget;
   GTK asks its parent's **`GtkLayoutManager`** (Section 9) to
   recalculate positions if needed, then issues drawing commands via
   **`GtkSnapshot`** (Section 8), styled according to whatever
   **`GtkCssProvider`** rules match its current CSS classes.
7. If this button's existence depended on a list of data (say, it's a
   row generated by a `GtkListView`), all of the above happens against a
   widget that a **`GtkListItemFactory`** created on demand from a
   **`GListModel`** (Section 6) — and GTK only created that widget in the
   first place because the row scrolled into view.

Every layer is doing one job. None of them know much about the layers two
steps away — Gdk doesn't know about CSS, layout managers don't know about
signals, models don't know about rendering. That separation is what makes
the system composable instead of monolithic, and it's also exactly why
the API surface, while large, decomposes into a small number of repeating
patterns once you know where to look.
