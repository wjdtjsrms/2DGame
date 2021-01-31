#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence
{
	class Child;

	class Parent
	{
	public:
		enum Mode {
			MODE_1P,
			MODE_2P,

			MODE_NONE,
		};

	public:
		void update();
		Mode mode() const;
		void setMode(Mode);

		// ½Ì±ÛÅæ °ü·Ã ÇÔ¼öµé
		static void create();
		static void destroy();
		static Parent* instance();

	private:
		Parent();
		~Parent();

		Mode mMode;
		Child* mChild;
		static Parent* mInstance;
	};
} 
#endif
