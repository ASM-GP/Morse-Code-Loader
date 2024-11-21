/***********************************************************************************************
*   \brief is_same structs are to check if the genetric types are the same
*   \note  does not need to be declared, \example -> is_same example;
************************************************************************************************/

template <typename T, typename U>
struct is_same {
  static constexpr bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static constexpr bool value = true;
};

/***********************************************************************************************
*   \brief lambda that returns the type that is in the 
*   \note This piece of code is universal for anything like classes, templates, etc.
************************************************************************************************/

template <typename T> 
static auto getType(T&& var) -> decltype(var) {return var;}

/**********************************************************************************************
*   \brief automatically casts void* into perferred type
***********************************************************************************************/

template <typename T> 
static T cast(void* ptr) {return *static_cast<T*>(ptr);}