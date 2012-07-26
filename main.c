/* 
   $(CC) main.c `pkg-config ecore --cflags --libs` `pkg-config ecore-evas --cflags --libs` `pkg-config emotion --cflags --libs`  `pkg-config evas --cflags --libs` 
 */
#include <Evas.h>
#include <Ecore_Evas.h>
#include <Emotion.h>

#define WINDOW_WIDTH 300 
#define WINDOW_HEIGHT 200

static Eina_Bool key_cb(void *data, int type, void *event) {
  Evas_Object *video = (Evas_Object *)data;
  Ecore_Event_Key *kev = (Ecore_Event_Key*)event;
  const Evas_Map *const_m = evas_object_map_get(video);
  Evas_Map *m = evas_map_dup(const_m);

  static double dx = 0;
  static double dy = 0;
  static double dz = 0;

  if(strcmp(kev->keyname, "Right") == 0) {
    dx += 0.1;
  } else if(strcmp(kev->keyname, "Left") == 0) {
    dx -= 0.1;
  } else if(strcmp(kev->keyname, "Up") == 0) {
    dy += 0.1;
  } else if(strcmp(kev->keyname, "Down") == 0) {
    dy -= 0.1;
  } else {
      printf("name:%s\n", kev->keyname);
      evas_map_free(m);
      return EINA_FALSE;
  }

  printf("dx:%f dy:%f dz:%f\n", dx, dy, dz);
  evas_map_util_3d_rotate(m, dx, dy, dz, 0, 0, 0);
  evas_object_map_set(video, m);
  evas_map_free(m);
  return EINA_TRUE;
}

static void
main_delete_request(Ecore_Evas *ee)
{
   ecore_main_loop_quit();
}

int main(int argc, char** argv) {
  Eina_Bool ret;
  Ecore_Evas *ecore_evas;
  Evas *evas;
  Evas_Object *bg, *video;
  Evas_Map *m;

  if(argc != 2) {
    fprintf(stderr, "Usage: %s moviefile\n", argv[0]);
    return 1;
  }

  ecore_init();
  ecore_evas_init();
  emotion_init();

  ecore_evas = ecore_evas_new
    ("software_x11", 300, 200, WINDOW_WIDTH, WINDOW_HEIGHT, NULL);
   if (!ecore_evas)
     goto end;

  ecore_evas_callback_delete_request_set(ecore_evas, main_delete_request);
  // ecore_evas_callback_resize_set(ecore_evas, main_resize);
  ecore_evas_title_set(ecore_evas, "Evas Media Test Program");
  // ecore_evas_name_class_set(ecore_evas, "evas_media_test", "main");
  ecore_evas_show(ecore_evas);

  evas = ecore_evas_get(ecore_evas);

  bg = evas_object_rectangle_add(evas);
  evas_object_color_set(bg, 0x00, 0x00, 0x00, 0xff);
  evas_object_move(bg, 0, 0);
  evas_object_resize(bg, WINDOW_WIDTH, WINDOW_HEIGHT);
  evas_object_show(bg);

  video = emotion_object_add(evas);
  evas_object_move(video, 0, 0);
  evas_object_resize(video, WINDOW_WIDTH, WINDOW_HEIGHT);

  ret = emotion_object_init(video, "gstreamer");
  if(ret != EINA_TRUE) {
    fprintf(stderr, "faild to emotion_object_init\n");
    goto end;
  }
  ret = emotion_object_file_set(video, argv[1]);
  if(ret != EINA_TRUE) {
    fprintf(stderr, "emotion_object_file_set\n");
    goto end;
  }
  emotion_object_play_set(video, EINA_TRUE);
  evas_object_show(video);

  m = evas_map_new(4);
  evas_map_util_points_populate_from_object(m, video);
  // 2d rotate
  // evas_map_util_rotate(m, 90, 0 + (WINDOW_WIDTH/2), 0 + (WINDOW_HEIGHT/2));
  // 3d rotate
  //evas_map_util_3d_rotate(m, 10.0, 10.0, 0.0, 0, 0, 0);
  evas_object_map_set(video, m);
  evas_object_map_enable_set(video, EINA_TRUE);

  ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, key_cb, video);

  ecore_main_loop_begin();

 end:
  if(ecore_evas)
    ecore_evas_free(ecore_evas);

  evas_map_free(m);
  emotion_shutdown();
  ecore_evas_shutdown();
  ecore_shutdown();

  return 0;
}
