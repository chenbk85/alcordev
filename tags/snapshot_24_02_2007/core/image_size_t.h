#ifndef image_size_t_H_INCLUDED
#define image_size_t_H_INCLUDED
//----------------------------------------------------------------++
namespace all { namespace core {
//----------------------------------------------------------------++
struct image_size_t
	{
	///
	std::size_t height;
	///
	std::size_t width;
	///
	std::size_t channels;
	///
	std::size_t memory_size;
	};
//----------------------------------------------------------------++
//template<typename T>
struct image_info_t
	{
    ///
    //typedef T value_type;
	///
	int height;
	///
	int width;
	///
	int channels;
    ///
    std::size_t memory_size;
	};
//----------------------------------------------------------------++
	}}//all::core
//----------------------------------------------------------------++
#endif //image_size_t_H_INCLUDED