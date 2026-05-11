#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <calculator.h>

static const char* error_code(calc_error_t error) {
   switch (error) {
      case CALC_ERR_OK:              return "No error";
      case CALC_ERR_INVALID_TOKEN:   return "Invalid token";
      case CALC_ERR_UNBALANCED_PAREN:return "Unbalanced parenthesis";
      case CALC_ERR_OVERFLOW:        return "Numeric overflow";
      case CALC_ERR_NULL_POINTER:    return "Null Pointer";
      case CALC_ERR_DIVIDE_BY_ZERO:   return "Cannot divide by zero";
      default:                  return "Unknown error";
   }
}