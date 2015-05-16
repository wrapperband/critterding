#ifndef BE_SCENE_H_INCLUDED
#define BE_SCENE_H_INCLUDED

class BeScene {
	public:
		BeScene() : m_exitscene(false)
		{
		};
		virtual ~BeScene() {};
		void run()
		{
			init();
			while(!m_exitscene)
			{
				process();
			}
			clean();
		}
protected:
	virtual void quit() 
	{ 
		m_exitscene = true; 
	}
private:
		virtual void init() {};
		virtual void process() {};
		virtual void clean() {};
		bool m_exitscene;
};

#endif	//BE_SCENE_H_INCLUDED
