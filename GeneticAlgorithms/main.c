#include "bits/pattern.h"
#include "collections/list.h"
#include "collections/set.h"
#include "classifier.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	classifier_B classifier =classifier_new(10,10);
	classifier_print(classifier);
}
