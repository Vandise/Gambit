#include "cspec.h"
#include "cspec_output_header.h"
#include "cspec_output_verbose.h"
#include "cspec_output_unit.h"

#include "util/linked_list.h"

extern int status;

DESCRIBE(utility_generic_linked_list_list_init, "Generic Linked List Utility - list_init")

	IT("Can initialize a new list")
	  List list;
	  list_init(&list, free);

		SHOULD_NOT_EQUAL( NULL, &list )
	END_IT

	IT("Initializes the size to zero")
	  List list;
	  list_init(&list, free);

		SHOULD_EQUAL( list_size(&list), 0 )
	END_IT

  IT("Sets delete to point to a function")
	  List list;
	  list_init(&list, free);
	  SHOULD_EQUAL( list.destroy, free )
  END_IT

  IT("Sets the head to NULL")
	  List list;
	  list_init(&list, free);

    SHOULD_BE_NULL(list.head)
  END_IT

  IT("Sets the tail to NULL")
	  List list;
	  list_init(&list, free);

    SHOULD_BE_NULL(list.tail)
  END_IT

END_DESCRIBE

DESCRIBE(utility_generic_linked_list_list_ins_next, "Generic Linked List Utility - list_ins_next")

	IT("Sets the list size")
	  List list;
	  list_init(&list, free);

	  int *data = (int *)malloc(sizeof(int));
	  *data = 10;

    list_ins_next(&list, NULL, data);
		SHOULD_EQUAL( list_size(&list), 1 )

		list_destroy(&list);
	END_IT

  IT("Sets the head and tail when no items are present")
	  List list;
	  list_init(&list, free);

	  int *data = (int *)malloc(sizeof(int));
	  *data = 10;

    list_ins_next(&list, NULL, data);

    SHOULD_EQUAL( list_head(&list)->data, data)
    SHOULD_EQUAL( list_tail(&list)->data, data)

    list_destroy(&list);
  END_IT

  IT("Sets the tail as the first item in the list")
	  List list;
	  list_init(&list, free);

	  int *item_one = (int *)malloc(sizeof(int));
	  int *item_two = (int *)malloc(sizeof(int));
	  *item_one = 10;
	  *item_two = 12;

    list_ins_next(&list, NULL, item_one);
    list_ins_next(&list, NULL, item_two);

    SHOULD_EQUAL( list_head(&list)->data, item_two)
    SHOULD_EQUAL( list_tail(&list)->data, item_one)

    list_destroy(&list);
  END_IT

  IT("Sets the element after the specified item")
	  List list;
	  list_init(&list, free);

	  int *item_one = (int *)malloc(sizeof(int));
	  int *item_two = (int *)malloc(sizeof(int));
	  *item_one = 10;
	  *item_two = 12;

    list_ins_next(&list, NULL, item_one);

    ListElmt *list_item = list_tail(&list);

    list_ins_next(&list, list_item, item_two);

    SHOULD_EQUAL( list_head(&list)->data, item_one)
    SHOULD_EQUAL( list_tail(&list)->data, item_two)

    list_item = NULL;

    list_destroy(&list);
  END_IT

END_DESCRIBE

DESCRIBE(utility_generic_linked_list_list_rem_next, "Generic Linked List Utility - list_rem_next")

  IT("Returns a status code of 0 on success")
    List list;
    ListElmt *element;
    int *data, i;

    list_init(&list, free);

    for (i = 10; i > 0; i--) {
      data = (int *)malloc(sizeof(int));
      *data = i;
      list_ins_next(&list, NULL, data);
    }

    element = list_head(&list);

    for (i = 0; i < 3; i++)
      element = list_next(element);

    data = list_data(element);

    SHOULD_EQUAL( list_rem_next(&list, element, (void **)&data), 0 )
    element = NULL;
    free(data);
    list_destroy(&list);
  END_IT

  IT("Decrements the size by 1")
    List list;
    ListElmt *element;
    int *data, i;

    list_init(&list, free);

    for (i = 10; i > 0; i--) {
      data = (int *)malloc(sizeof(int));
      *data = i;
      list_ins_next(&list, NULL, data);
    }

    element = list_head(&list);

    for (i = 0; i < 3; i++)
      element = list_next(element);

    data = list_data(element);
    list_rem_next(&list, element, (void **)&data);

    SHOULD_EQUAL( list_size(&list), 9 );
    free(data);
    element = NULL;
    list_destroy(&list);
  END_IT

END_DESCRIBE

DESCRIBE(utility_generic_linked_list_list_destroy, "Generic Linked List Utility - list_destroy")

  IT("Clears the list")
    List list;
    int *data, i;

    list_init(&list, free);

    for (i = 10; i > 0; i--) {
      data = (int *)malloc(sizeof(int));
      *data = i;
      list_ins_next(&list, NULL, data);
    }

    list_destroy(&list);
    SHOULD_EQUAL( list_size(&list), 0 );
  END_IT

END_DESCRIBE

DESCRIBE(utility_generic_linked_list, "Generic Linked List Utility")

  IT("defines the List struct")
    List list;
    SHOULD_EQUAL(&list, &list);
  END_IT

  IT("defines the ListElmt struct")
    ListElmt el;
    SHOULD_EQUAL(&el, &el);
  END_IT

  status += CSpec_Run( DESCRIPTION( utility_generic_linked_list_list_init ), CSpec_NewOutputVerbose() );
  status += CSpec_Run( DESCRIPTION( utility_generic_linked_list_list_ins_next ), CSpec_NewOutputVerbose() );
  status += CSpec_Run( DESCRIPTION( utility_generic_linked_list_list_rem_next ), CSpec_NewOutputVerbose() );
  status += CSpec_Run( DESCRIPTION( utility_generic_linked_list_list_destroy ), CSpec_NewOutputVerbose() );
END_DESCRIBE