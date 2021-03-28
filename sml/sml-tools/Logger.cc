#include "Logger.hh"
#include <sml/sml-tools/Settings.hh>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/any.hpp>
#include <boost/filesystem.hpp>

map<string, Logger*> LoggerManager::loggers;

void LoggerManager::init(){
	const int LogOn =Settings::get<int>("log_nmm");
	if(LogOn != 1) return;
	for(auto&kv: loggers){
		kv.second->createFiles();
		kv.second->writeHeader();
	}
}

void LoggerManager::step(){
	const int LogOn =Settings::get<int>("log_nmm");
	if(LogOn != 1) return;
	for(auto&kv: loggers ){
		kv.second->writeContent();
	}
}

void LoggerManager::end(){
	const int LogOn =Settings::get<int>("log_nmm");
	if(LogOn != 1) return;
	for(auto&kv: loggers ){
		kv.second->closeFiles();
	}
}




void Logger::closeFiles(){
	if(LogOn != 1) return;
		for(auto& kv : raw_files) kv.second->close();
}
void Logger::createFiles(){
	// Check if folder exists 
	string folder_path = Path+Folder;
	const char* path = folder_path.c_str();
	boost::filesystem::path dir(path);
	if(boost::filesystem::create_directory(dir))
	{
	    std::cerr<< "Directory Created: "<< path <<std::endl;
	}
	// Creating log files 
	print_debug("--creating log file");
	int i=1;
	string out = boost::lexical_cast<std::string>(i);
    if(LogOverwrite==0){
        while(fexists(Path+Folder+"/"+raw_filesname[0]+out)){
        	out = boost::lexical_cast<std::string>(i);
            i++;
            print_debug("--found existing log skipping");
        }

    }
    else{
        i = LogOverwrite;
		out = boost::lexical_cast<std::string>(i);
	}
	std::ofstream *file;
	for (auto & it : raw_filesname){
		file = new ofstream();
		raw_files[it] = file;
		raw_files[it]->open(Path+Folder+"/"+it+out);
	}
}

void JointLogger::writeHeader(){
	print_debug("--writing logger header");
	if(LogOn != 1) return;
	
	print_debug("--writing joint logger header");
	for(auto& joint : body.joints)
	{
		*raw_files["joints_angle"] << "ANGLE_" << joint->getName() << " ";
	}

	for(auto &kv : raw_files)
		*kv.second << endl;	
}
void JointLogger::writeContent(){
	print_debug("--writing logger content");
	if(LogOn != 1) return;
	
	for (
	        unsigned input=INPUT::FIRST_JOINT,
	                 output=OUTPUT::FIRST_JOINT; 
	        output<=OUTPUT::LAST_JOINT; 
	        output++,
	        input++
	        ){
	        	*raw_files["joints_angle"] << body.Input[input] << " ";
	}
	print_debug("--writing sensors");        
	for(auto &kv : raw_files)
		*kv.second << endl;
}

void MusculoSkeletalLogger::writeHeader(){
	print_debug("--writing logger header");
	if(LogOn != 1) return;
	
	int id=0;
	for(auto& kv : body.muscles)
	{
		Muscle * mu = kv.second;
		*raw_files["muscles_force"] << mu->getName() << " ";
		*raw_files["muscles_v_CE"] << mu->getName() << " ";
		*raw_files["muscles_F_CE"] << mu->getName() << " ";
		*raw_files["muscles_f_v"] << mu->getName() << " ";
		*raw_files["muscles_length"] << mu->getName() << " ";
		*raw_files["muscles_l_se"] << mu->getName() << " ";
		*raw_files["muscles_activity"] << mu->getName() << " " ;
		*raw_files["muscles_noise"] << mu->getName() << " " ;
		*raw_files["muscles_stim"] << mu->getName() << " " ;
        id++;
	}
	print_debug("--writing muscles logger header");
	for(auto& joint : body.joints)
	{
		*raw_files["joints_force"] << "TORQUE_" << joint->getName() << " ";
		*raw_files["joints_position_y"] << joint->getName() << " ";
		*raw_files["joints_position_z"] << joint->getName() << " ";
	}
    print_debug("--writing joint logger header");
	for(auto& kv :this->body.sensors)
	{
		*raw_files["sensors_activity"] << kv.first << " ";
	}
	print_debug("--writing sensors_activity logger header");

	for(auto &kv : raw_files)
		*kv.second << endl;
    
}

void MusculoSkeletalLogger::writeContent(){
	print_debug("--writing logger content");
	if(LogOn != 1) return;
	for(auto& kv : body.muscles)
	{
		Muscle * mu = kv.second;
		*raw_files["muscles_force"] << mu->getForce() << " ";
		*raw_files["muscles_v_CE"] << mu->getV_CE() << " ";
		*raw_files["muscles_F_CE"] << mu->getMuscleActiveForce() << " ";
		*raw_files["muscles_length"] << mu->getL_CE() << " ";
		*raw_files["muscles_l_se"] << mu->getL_SE() << " ";
		*raw_files["muscles_f_v"] << mu->getF_v() << " ";
		*raw_files["muscles_activity"] << mu->getA() << " " ;
		*raw_files["muscles_noise"] << mu->getNoise() << " " ;
		*raw_files["muscles_stim"] << mu->stim << " " ;
	}
	print_debug("--writing muscles");
	// for(auto& joint : body.joints)
	// {
	// 	*raw_files["joints_force"] << sml::Output[joint_tau] << " ";
	// 	*raw_files["joints_angle"] << joint->getAngle() << " ";
	// 	//*raw_files["joints_position_z"] << this->body.getFromDef(joint->getName())->getPosition()[2]-this->body.getFromDef("REGIS")->getPosition()[2] << " ";
	// 	//*raw_files["joints_position_y"] << this->body.getFromDef(joint->getName())->getPosition()[1]-this->body.getFromDef("REGIS")->getPosition()[1] << " ";
	// }
	for (
	        unsigned input=INPUT::FIRST_JOINT,
	                 output=OUTPUT::FIRST_JOINT; 
	        output<=OUTPUT::LAST_JOINT; 
	        output++,
	        input++
	        ){
	            *raw_files["joints_force"] << body.Output[output] << " ";
	        	*raw_files["joints_angle"] << body.Input[input] << " ";
	}
	print_debug("--writing joints");
	for(auto& kv :this->body.sensors)
	{
		*raw_files["sensors_activity"] << kv.second->get() << " ";
	}
	print_debug("--writing sensors");        
	for(auto &kv : raw_files)
		*kv.second << endl;
}

void ReflexControllerLogger::writeHeader()
{
	if(LogOn != 1) return;
	for(auto& kv : controller.motoneurons)
	{
		Motoneuron * mu = kv.second;
		*raw_files["motoneurons_activity"] << mu->getName() << " " ;
	}
	for(auto& kv :controller.INsens)
	{
        auto i = &kv - &controller.INsens[0];
		*raw_files["feedbacks"] << SENSORY_INTERNEURON::toString(i) << " ";
		*raw_files["cpgs"] << SENSORY_INTERNEURON::toString(i) << " ";
	}
	MUSCLES_TYPES muscles = controller.inputs.get<MUSCLES_TYPES>("muscles");
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(auto& kv: muscles){
            std::string MN = kv.first;
            if(SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN]){
                *raw_files["interneurons"] << SENSORY_INTERNEURON::toString(INsen) << " ";
            }   
        }
    }
	for(auto &kv : raw_files)
		*kv.second << endl;
}

void ReflexControllerLogger::writeContent()
{
	if(LogOn != 1) return;
	print_debug("writing motoneurons");
	for(auto& kv : controller.motoneurons)
	{
		Motoneuron * mn = kv.second;
		*raw_files["motoneurons_activity"] << ((Entity *)mn)->get() << " " ;
	}
	print_debug("writing cpgs");
	for(auto& kv :controller.INcpgs)
	{
		*raw_files["cpgs"] << kv->get() << " ";
	}
	print_debug("writing insens");
	for(auto& kv :controller.INsens)
	{
		*raw_files["feedbacks"] << kv->get() << " ";
	}
	print_debug("writing interneurons");
	MUSCLES_TYPES muscles = controller.inputs.get<MUSCLES_TYPES>("muscles");
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(auto& kv: muscles){
            std::string MN = kv.first;
            if(SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN]){
                *raw_files["interneurons"] << controller.__INsen_MN[INsen][MN]->input->get() << " ";
            }   
        }
    }
    
	for(auto &kv : raw_files)
		*kv.second << endl;
    
}

