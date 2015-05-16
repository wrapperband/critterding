#include "speciesview.h"
#include "gui/button.h"
#include "gui/textprinter.h"
#include "scenes/critterding/entities/genotypes.h"
#include <GL/glew.h>

Speciesview::Speciesview()
 : Panel(false)
 , m_recalc_interval(25) // fixme feed fps to this
 , m_recalc_count(m_recalc_interval)
 , m_num_species_buttons(0)
 , m_colspacer(15)
{
	genotypes = Genotypes::Instance();
	colormode = Settings::Instance()->getCVarPtr("colormode");

	m_localposition.set(2, 24);

	m_dimensions.set(200, 300);
	
	m_active = false;
	isMovable = true;

	titlebar = 25;
	v_space = -10 + titlebar;
	rowspacer = 4;
	qwidth = 25;
	qheight = 10;
	
	titlePos = "#";
	titleColor = "Clr";
	titleNum = "Num";
	titleAd = "AD";
	titleNeurons = "Neu";
	titleSynapses = "Syn";
	titleBodyparts = "Bp";
	
	titlePosW = textprinter->getWidth( titlePos );
	titleColorW = textprinter->getWidth( titleColor );
	titleNumW = textprinter->getWidth( titleNum );
	titleAdW = textprinter->getWidth( titleAd );
	titleNeuronsW = textprinter->getWidth( titleNeurons );
	titleSynapsesW = textprinter->getWidth( titleSynapses );
	titleBodypartsW = textprinter->getWidth( titleBodyparts );

	BeWidgetPtr w = addWidgetButton( "speciesview_close", Vector2i(2, 2), Vector2i(11, 11), "x", BeCommand("gui_togglepanel", "speciesview"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

void Speciesview::recalculate()
{
		highestCount = 0;
		highestAD = 0;
		highestNeurons = 0;
		highestSynapses = 0;
		highestBodyparts = 0;

		indices.clear();
// 		for ( unsigned int i(0); i < genotypes->list.size(); ++i )
// 			indices.push_back(0);
		
// 		vector<int> indices ( genotypes->list.size(), 0 );
		for ( unsigned int i = 0; i < genotypes->list.size(); ++i )
		{
			indices.push_back(i);
			const Genotype* g2 = genotypes->list[i];
			
// 			indices[i] = i;
			
			if ( g2->count > highestCount )
				highestCount = g2->count;
			
			if ( g2->adamdist > highestAD )
				highestAD = g2->adamdist;
			
			if ( g2->brainzArch->ArchNeurons.size() > highestNeurons )
				highestNeurons = g2->brainzArch->ArchNeurons.size();
			
			// calc total syns
			unsigned int totalSyns = g2->brainzArch->totalSynapses;
// 			for ( unsigned int j = 0; j < g2->brainzArch->ArchNeurons.size(); ++j )
// 				totalSyns += g2->brainzArch->ArchNeurons[j].ArchSynapses.size();
// 				totalSyns += g2->brainzArch->totalSynapses;
			
			if ( totalSyns > highestSynapses )
				highestSynapses = totalSyns;
			
			// FIXME dunno what this is heh disabled it
// 			if ( g2->bodyArch->archBodyparts.size() > highestNeurons )
// 				highestNeurons = g2->bodyArch->archBodyparts.size();
		}

	// sort results
		for ( int i = genotypes->list.size(); i>0; i--  )
			for ( int j = 0; j < i-1; ++j  )
				if ( genotypes->list[indices[j]]->count < genotypes->list[indices[j+1]]->count )
				{
					const auto keepI = indices[j];
					indices[j] = indices[j+1];
					indices[j+1] = keepI;
				}

// 		const int titleNumWH = textprinter->getWidth( highestCount );
// 		const int titleColorWH = qwidth;
// 		const int titlePosWH = textprinter->getWidth( genotypes->list.size() );
// 		const int titleAdWH = textprinter->getWidth( highestAD );
// 		const int titleNeuronsWH = textprinter->getWidth( highestNeurons );
// 		const int titleSynapsesWH = textprinter->getWidth( highestSynapses );
// 		const int titleBodypartsWH = textprinter->getWidth( highestBodyparts );
// 		
// 		int colw1 = titlePosW;
// 		if ( colw1 < titlePosWH ) colw1 = titlePosWH;
// 		int colw2 = titleColorW;
// 		if ( colw2 < titleColorWH ) colw2 = titleColorWH;
// 		int colw3 = titleNumW;
// 		if ( colw3 < titleNumWH ) colw3 = titleNumWH;
// 		int colw4 = titleAdW;
// 		if ( colw4 < titleAdWH ) colw4 = titleAdWH;
// 		int colw5 = titleNeuronsW;
// 		if ( colw5 < titleNeuronsWH ) colw5 = titleNeuronsWH;
// 		int colw6 = titleSynapsesW;
// 		if ( colw6 < titleSynapsesWH ) colw6 = titleSynapsesWH;
// 		int colw7 = titleBodypartsW;
// 		if ( colw7 < titleBodypartsWH ) colw7 = titleBodypartsWH;
// 
// 		const int m_colspacer = 15;
// 		const int col1 = m_colspacer;
// 		const int col2 = col1+m_colspacer + colw1;
// 		const int col3 = col2+m_colspacer + colw2;
// 		const int col4 = col3+m_colspacer + colw3;
// 		const int col5 = col4+m_colspacer + colw4;
// 		const int col6 = col5+m_colspacer + colw5;
// 		const int col7 = col6+m_colspacer + colw6;
// 		const int col8 = col7+m_colspacer + colw7;

		titleNumWH = textprinter->getWidth( highestCount );
		titleColorWH = qwidth;
		titlePosWH = textprinter->getWidth( genotypes->list.size() );
		titleAdWH = textprinter->getWidth( highestAD );
		titleNeuronsWH = textprinter->getWidth( highestNeurons );
		titleSynapsesWH = textprinter->getWidth( highestSynapses );
		titleBodypartsWH = textprinter->getWidth( highestBodyparts );
		
		colw1 = titlePosW;
		if ( colw1 < titlePosWH ) colw1 = titlePosWH;
		colw2 = titleColorW;
		if ( colw2 < titleColorWH ) colw2 = titleColorWH;
		colw3 = titleNumW;
		if ( colw3 < titleNumWH ) colw3 = titleNumWH;
		colw4 = titleAdW;
		if ( colw4 < titleAdWH ) colw4 = titleAdWH;
		colw5 = titleNeuronsW;
		if ( colw5 < titleNeuronsWH ) colw5 = titleNeuronsWH;
		colw6 = titleSynapsesW;
		if ( colw6 < titleSynapsesWH ) colw6 = titleSynapsesWH;
		colw7 = titleBodypartsW;
		if ( colw7 < titleBodypartsWH ) colw7 = titleBodypartsWH;

// 		m_colspacer = 15;
		col1 = m_colspacer;
		col2 = col1+m_colspacer + colw1;
		col3 = col2+m_colspacer + colw2;
		col4 = col3+m_colspacer + colw3;
		col5 = col4+m_colspacer + colw4;
		col6 = col5+m_colspacer + colw5;
		col7 = col6+m_colspacer + colw6;
		col8 = col7+m_colspacer + colw7;

// 		for ( unsigned int i=0; i < m_num_species_buttons; ++i  )
// 		{
// 			std::stringstream b_name;
// 			b_name << "b" << i;
// 			auto e( get(b_name.str()) );
// 			if ( e )
// 				e->deactivate();
// // 			deactivate()
// 		}

		// add buttons if we don't have enough
		v_space = -10 + titlebar;
		for ( unsigned int i=0; i < genotypes->list.size() || i < m_num_species_buttons; ++i  )
		{
			v_space += qheight + rowspacer; 

			std::stringstream b_name_stream;
			b_name_stream << "b" << i;
			const auto& b_name( b_name_stream.str() );

			auto e( get(b_name) );
// 			if ( e )
// 				e->deactivate();

			if ( m_num_species_buttons <= i )
			{
// 				std::cout << "adding button " << b_name << std::endl;
// 				std::cout << " m_num_species_buttons " << m_num_species_buttons << std::endl;
// 				std::cout << " genotypes->list.size()" << genotypes->list.size() << std::endl;
				
				e = addWidgetButton( b_name, Vector2i(col2-1, v_space), Vector2i(qwidth+1, qheight+1), "", BeCommand("cs_select_species", indices[i]), EVENT_NOREPEAT, 0 );
				++m_num_species_buttons;
			}
			
			boost::shared_ptr<BeWidgetButton> e1 = boost::dynamic_pointer_cast<BeWidgetButton>(e);
			if ( e1 )
			{
	// 			auto e( get(b_name) );
				if ( i >= genotypes->list.size() )
				{
					e1->deactivate();
				}
				else
				{
					e1->activate();
				}
				e1->getCommand(1)->m_argInt = indices[i];
				e1->m_localposition.set(col2-1, v_space);
				
			}
			
		}
		updateAbsPosition();
		
	m_recalc_count = 0;
}

void Speciesview::draw()
{
	++m_recalc_count;
	
	if (m_active)
	{
	// initialize sort indices
	// at the same time find the highest Counts for all columns

// 		if ( m_recalc_count >= m_recalc_interval )
		if ( m_recalc_count >= m_recalc_interval || indices.size() != genotypes->list.size() )
		{
			recalculate();
		}


		v_space = -10 + titlebar;

		m_dimensions.set(col8, (genotypes->list.size() * (qheight+rowspacer)) + rowspacer + titlebar);
		drawBackground();
		drawBorders();
		drawChildren();

		const auto y( getPosition().m_y+19 );
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		textprinter->print( Vector2i(getPosition().m_x+col1, y), titlePos.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col2, y), titleColor.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col3, y), titleNum.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col4, y), titleAd.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col5, y), titleNeurons.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col6, y), titleSynapses.c_str() );
		textprinter->print( Vector2i(getPosition().m_x+col7, y), titleBodyparts.c_str() );
		
		
		auto vspace2 = v_space;
// 		auto vspace3 = v_space;
		
// 		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		for ( unsigned int i=0; i < genotypes->list.size(); ++i  )
		{
			v_space += qheight + rowspacer; 
			
			const Genotype* g = genotypes->list[indices[i]];
// 			const Genotype* g2 = genotypes->list[i];

			const auto y( getPosition().m_y+v_space+9 );

			textprinter->print( getPosition().m_x+col1, y, i+1 );
			textprinter->print( getPosition().m_x+col3, y, g->count );
			textprinter->print( getPosition().m_x+col4, y, g->adamdist );
			textprinter->print( getPosition().m_x+col5, y, g->brainzArch->ArchNeurons.size() );

// 			// total syns
// 			unsigned int totalSyns = g->brainzArch->totalSynapses;
// 			
// 			if ( totalSyns == 0 )
// 				for ( unsigned int j = 0; j < g->brainzArch->ArchNeurons.size(); ++j )
// 					totalSyns += g->brainzArch->ArchNeurons[j].ArchSynapses.size();
// 
// 			g->brainzArch->totalSynapses = totalSyns;

			textprinter->print( getPosition().m_x+col6, y, g->brainzArch->totalSynapses );
			textprinter->print( getPosition().m_x+col7, y, g->bodyArch->archBodyparts.size() + g->bodyArch->archMouths.size() );
		}

// 		glBegin(GL_LINES);
// 		for ( unsigned int i=0; i < genotypes->list.size(); ++i  )
// 		{
// 			vspace3 += qheight + rowspacer; 
// 			const auto y_vspace( getPosition().m_y+vspace3 );
// 
// 			glVertex2f(getPosition().m_x+col2,         y_vspace+qheight);
// 			glVertex2f(getPosition().m_x+col2,         y_vspace);
// 
// 			glVertex2f(getPosition().m_x+col2,         y_vspace);
// 			glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace);
// 
// 			glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace);
// 			glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace+qheight);
// 
// 			glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace+qheight);
// 			glVertex2f(getPosition().m_x+col2,         y_vspace+qheight);
// 		}
// 		glEnd();
		
		
		glBegin(GL_QUADS);
		for ( unsigned int i=0; i < genotypes->list.size(); ++i  )
		{
			vspace2 += qheight + rowspacer; 
			const Genotype* g = genotypes->list[indices[i]];
			const auto y_vspace( getPosition().m_y+vspace2 );

			if ( *colormode )
				glColor4f(g->speciescolor.r(), g->speciescolor.g(), g->speciescolor.b(), g->speciescolor.a());
			else
				glColor4f(g->bodyArch->color.r(), g->bodyArch->color.g(), g->bodyArch->color.b(), g->bodyArch->color.a());

				glVertex2f(getPosition().m_x+col2,         y_vspace+qheight);
				glVertex2f(getPosition().m_x+col2,         y_vspace);
				glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace);
				glVertex2f(getPosition().m_x+col2+qwidth,  y_vspace+qheight);
		}
		glEnd();

	}
}

Speciesview::~Speciesview()
{
}


























// #include "speciesview.h"
// 
// Speciesview::Speciesview() : Panel(false)
// {
// 	genotypes = Genotypes::Instance();
// 	colormode = Settings::Instance()->getCVarPtr("colormode");
// 
// 	m_localposition.set(2, 24);
// 	m_dimensions.set(200, 300);
// 	
// 	m_active = false;
// 	isMovable = true;
// 
// 	titlebar = 25;
// 	v_space = -10 + titlebar;
// 	rowspacer = 4;
// 	qwidth = 25;
// 	qheight = 10;
// 	
// 	titlePos = "#";
// 	titleColor = "Color";
// 	titleNum = "Population";
// 	titleAd = "Adam Distance";
// 	titleNeurons = "Neurons";
// 	titleSynapses = "Synapses";
// 	titleBodyparts = "Bodyparts";
// 	
// 	titlePosW = textprinter->getWidth( titlePos );
// 	titleColorW = textprinter->getWidth( titleColor );
// 	titleNumW = textprinter->getWidth( titleNum );
// 	titleAdW = textprinter->getWidth( titleAd );
// 	titleNeuronsW = textprinter->getWidth( titleNeurons );
// 	titleSynapsesW = textprinter->getWidth( titleSynapses );
// 	titleBodypartsW = textprinter->getWidth( titleBodyparts );
// }
// 
// void Speciesview::draw()
// {
// 	if (m_active)
// 	{
// 		unsigned int highestCount = 0;
// 		unsigned int highestAD = 0;
// 		unsigned int highestNeurons = 0;
// 		unsigned int highestSynapses = 0;
// 		unsigned int highestBodyparts = 0;
// 
// 	// initialize sort indices
// 	// at the same time find the highest Counts for all columns
// 		vector<int> indices ( genotypes->list.size(), 0 );
// 		for ( unsigned int i = 0; i < genotypes->list.size(); i++ )
// 		{
// 			indices[i] = i;
// 			
// 			if ( genotypes->list[i]->count > highestCount )
// 				highestCount = genotypes->list[i]->count;
// 			if ( genotypes->list[i]->adamdist > highestAD )
// 				highestAD = genotypes->list[i]->adamdist;
// 			if ( genotypes->list[i]->brainzArch->ArchNeurons.size() > highestNeurons )
// 				highestNeurons = genotypes->list[i]->brainzArch->ArchNeurons.size();
// 			// calc total syns
// 			unsigned int totalSyns = 0;
// 			for ( unsigned int j = 0; j < genotypes->list[i]->brainzArch->ArchNeurons.size(); j++ )
// 				totalSyns += genotypes->list[i]->brainzArch->ArchNeurons[j].ArchSynapses.size();
// 			if ( totalSyns > highestSynapses )
// 				highestSynapses = totalSyns;
// 			if ( genotypes->list[i]->bodyArch->archBodyparts.size() > highestNeurons )
// 				highestNeurons = genotypes->list[i]->bodyArch->archBodyparts.size();
// 		}
// 
// 	// sort results
// 		for ( int i = genotypes->list.size(); i>0; i--  )
// 			for ( int j = 0; j < i-1; j++  )
// 				if ( genotypes->list[indices[j]]->count < genotypes->list[indices[j+1]]->count )
// 				{
// 					unsigned keepI	= indices[j];
// 					indices[j]	= indices[j+1];
// 					indices[j+1]	= keepI;
// 				}
// 	
// 		int titleNumWH = textprinter->getWidth( highestCount );
// 		int titleColorWH = qwidth;
// 		int titlePosWH = textprinter->getWidth( genotypes->list.size() );
// 		int titleAdWH = textprinter->getWidth( highestAD );
// 		int titleNeuronsWH = textprinter->getWidth( highestNeurons );
// 		int titleSynapsesWH = textprinter->getWidth( highestSynapses );
// 		int titleBodypartsWH = textprinter->getWidth( highestBodyparts );
// 		
// 		int colw1 = titlePosW;
// 		if ( colw1 < titlePosWH ) colw1 = titlePosWH;
// 		int colw2 = titleColorW;
// 		if ( colw2 < titleColorWH ) colw2 = titleColorWH;
// 		int colw3 = titleNumW;
// 		if ( colw3 < titleNumWH ) colw3 = titleNumWH;
// 		int colw4 = titleAdW;
// 		if ( colw4 < titleAdWH ) colw4 = titleAdWH;
// 		int colw5 = titleNeuronsW;
// 		if ( colw5 < titleNeuronsWH ) colw5 = titleNeuronsWH;
// 		int colw6 = titleSynapsesW;
// 		if ( colw6 < titleSynapsesWH ) colw6 = titleSynapsesWH;
// 		int colw7 = titleBodypartsW;
// 		if ( colw7 < titleBodypartsWH ) colw7 = titleBodypartsWH;
// 
// 		int m_colspacer = 15;
// 		int col1 = m_colspacer;
// 		int col2 = col1+m_colspacer + colw1;
// 		int col3 = col2+m_colspacer + colw2;
// 		int col4 = col3+m_colspacer + colw3;
// 		int col5 = col4+m_colspacer + colw4;
// 		int col6 = col5+m_colspacer + colw5;
// 		int col7 = col6+m_colspacer + colw6;
// 		int col8 = col7+m_colspacer + colw7;
// 
// 		v_space = -10 + titlebar;
// 
// 		m_dimensions.set(col8, (genotypes->list.size() * (qheight+rowspacer)) + rowspacer + titlebar);
// 		drawBackground();
// 		drawBorders();
// 		drawChildren();
// 
// 		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
// 		textprinter->print( getPosition().m_x+col1, getPosition().m_y+19, titlePos.c_str() );
// 		textprinter->print( getPosition().m_x+col2, getPosition().m_y+19, titleColor.c_str() );
// 		textprinter->print( getPosition().m_x+col3, getPosition().m_y+19, titleNum.c_str() );
// 		textprinter->print( getPosition().m_x+col4, getPosition().m_y+19, titleAd.c_str() );
// 		textprinter->print( getPosition().m_x+col5, getPosition().m_y+19, titleNeurons.c_str() );
// 		textprinter->print( getPosition().m_x+col6, getPosition().m_y+19, titleSynapses.c_str() );
// 		textprinter->print( getPosition().m_x+col7, getPosition().m_y+19, titleBodyparts.c_str() );
// 		
// 		
// 		for ( unsigned int i=0; i < genotypes->list.size(); i++  )
// 		{
// 			v_space += qheight + rowspacer; 
// 
// 			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
// 			textprinter->print( getPosition().m_x+col1, getPosition().m_y+v_space+9, i+1 );
// 			textprinter->print( getPosition().m_x+col3, getPosition().m_y+v_space+9, genotypes->list[indices[i]]->count );
// 			textprinter->print( getPosition().m_x+col4, getPosition().m_y+v_space+9, genotypes->list[indices[i]]->adamdist );
// 			textprinter->print( getPosition().m_x+col5, getPosition().m_y+v_space+9, genotypes->list[i]->brainzArch->ArchNeurons.size() );
// 			// total syns
// 			unsigned int totalSyns = 0;
// 			for ( unsigned int j = 0; j < genotypes->list[i]->brainzArch->ArchNeurons.size(); j++ )
// 				totalSyns += genotypes->list[i]->brainzArch->ArchNeurons[j].ArchSynapses.size();
// 			textprinter->print( getPosition().m_x+col6, getPosition().m_y+v_space+9, totalSyns );
// 			textprinter->print( getPosition().m_x+col7, getPosition().m_y+v_space+9, genotypes->list[i]->bodyArch->archBodyparts.size() );
// 
// 			if ( *colormode )
// 				glColor4f(genotypes->list[indices[i]]->speciescolor.r(), genotypes->list[indices[i]]->speciescolor.g(), genotypes->list[indices[i]]->speciescolor.b(), genotypes->list[indices[i]]->speciescolor.a());
// 			else
// 				glColor4f(genotypes->list[indices[i]]->bodyArch->color.r(), genotypes->list[indices[i]]->bodyArch->color.g(), genotypes->list[indices[i]]->bodyArch->color.b(), genotypes->list[indices[i]]->bodyArch->color.a());
// 
// 			glBegin(GL_QUADS);
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space+qheight);
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space);
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space);
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space+qheight);
// 			glEnd();
// 			
// 			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
// 			glBegin(GL_LINES);
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space+qheight);
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space);
// 
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space);
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space);
// 
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space);
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space+qheight);
// 
// 				glVertex2f(getPosition().m_x+col2+qwidth,  getPosition().m_y+v_space+qheight);
// 				glVertex2f(getPosition().m_x+col2,         getPosition().m_y+v_space+qheight);
// 			glEnd();
// 		}
// 
// 	}
// }
// 
// Speciesview::~Speciesview()
// {
// }
