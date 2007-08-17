import Section
from GenFdsGlobalVariable import GenFdsGlobalVariable
import subprocess
from Ffs import Ffs
import os
from CommonDataClass.FdfClassObject import DataSectionClassObject
import shutil

class DataSection (DataSectionClassObject):
    def __init__(self):
        DataSectionClassObject.__init__(self)
        
    def GenSection(self, OutputPath, ModuleName, SecNum, keyStringList, FfsInf = None):
        #
        # Prepare the parameter of GenSection
        #
        if FfsInf != None:
            self.Alignment = FfsInf.__ExtendMarco__(self.Alignemnt)
            self.SecType = FfsInf.__ExtendMarco__(self.SecType)
            self.SectFileName = FfsInf.__ExtendMarco__(self.SectFileName)
        else:
            self.SectFileName = GenFdsGlobalVariable.ReplaceWorkspaceMarco(self.SectFileName)
        """Check Section file exist or not !"""

        if not os.path.exists(self.SectFileName):
            self.SectFileName = os.path.join (GenFdsGlobalVariable.WorkSpaceDir,
                                              self.SectFileName)
        if self.SecType == 'TE':
            TeFile = os.path.join( OutputPath, ModuleName + 'Te.raw')
            GenTeCmd = 'GenFW -t '    + \
                       ' -o '         + \
                        TeFile        + \
                        ' '           + \
                       GenFdsGlobalVariable.ExtendMarco(self.SectFileName)
            GenFdsGlobalVariable.CallExternalTool(GenTeCmd, "GenFw Failed !")
            """Copy Map file to Ffs output"""
            Filename = GenFdsGlobalVariable.ExtendMarco(self.SectFileName)
            if Filename[(len(Filename)-4):] == '.efi':
                MapFile = Filename.replace('.efi', '.map')
                if os.path.exists(MapFile):
                    CopyMapFile = os.path.join(OutputPath, ModuleName + '.map')
                    shutil.copyfile(MapFile, CopyMapFile)
            self.SectFileName = TeFile
           
            
                 
        OutputFile = os.path.join (OutputPath, ModuleName + 'SEC' + SecNum + Ffs.SectionSuffix.get(self.SecType))
        OutputFile = os.path.normpath(OutputFile)
        
        GenSectionCmd = 'GenSec -o '                                     + \
                         OutputFile                                      + \
                         ' -s '                                          + \
                         Section.Section.SectionType.get (self.SecType)  + \
                         ' '                                             + \
                         GenFdsGlobalVariable.ReplaceWorkspaceMarco(self.SectFileName)
                         
        """Copy Map file to Ffs output"""
        Filename = GenFdsGlobalVariable.ReplaceWorkspaceMarco(self.SectFileName)
        if Filename[(len(Filename)-4):] == '.efi':
             MapFile = Filename.replace('.efi', '.map')
             if os.path.exists(MapFile):
                 CopyMapFile = os.path.join(OutputPath, ModuleName + '.map')
                 shutil.copyfile(MapFile, CopyMapFile)
        #
        # Call GenSection
        #
        
        GenFdsGlobalVariable.CallExternalTool(GenSectionCmd, "GenSection Failed!")
        return OutputFile, self.Alignment
