/*=======================================================================================================================
= Sohag Developer Project
= This Project Aims To Create A PostgreSQL Tools
= To Help Users Using That Database
= At That Stage The Sohag Developer Is Generating Qt/C++ Files
= But Sohag Developer's Goal Is To Create A Compelete IDE For PostgreSQL
= Also The Project Will Be Extended To Include Other Databases As Long AS Possible
= For More Details Please Visit Sohag Developer website (http://sohag-developer.com/)
= This Software Is Licensed Under GPLv3 for more details visit (https://www.gnu.org/licenses/gpl-3.0.en.html)
= This Software is Sponsored By AlHuda Software الهدى للبرمجيات(http://alhuda-software.com/)
========================================================================================================================
*/


#ifndef SOHAGGEN_H
#define SOHAGGEN_H

#include <QString>
#include <QMessageBox>

enum {Database,Schema,Table,View};
enum {SchemaRole,TableTypeRole,PathRole,CatalogRole};
enum {HeaderFile,ImplementationFile,UiFile,ClassFiles,AllFile};
enum{CreateMode,EditMode,DeleteMode,OpenMode,ChangedStatus,UnChangedStatus};
enum {RowID,AttribDataDataType,AttribDataConstraint,AttribDataWidgetName,AttribDataWidgetValue,AttribDataTableType,AttribDataIndex,ClassNameClassInfo,BaseClassClassInfo,ClassTypeClassInfo,ProjectNameClassInfo,ProjectLocationClassInfo,CatalogNameClassInfo,SchemaNameClassInfo,TableNameClassInfo,TableTypeClassInfo,ModeItemStatus,StatusItemStatus,LastCheckedItemIndex};
enum{AttribColumnItem,AttribColumnLabelItem,AttribColumnWidgetItem,AttribColumnClassItem}; // Atrribute Columns
const QString sohagDevDBSystem="PG";
const double sohagDevVersion=1.0000;
const double sohagDevFileVersion=1.0000;
const double sohagDevBackwardsCompatibility=1.0000;
const double sohagDevBackwardsCompatibilityReadOnly=1.0000;
const QString sohagDeveloperSite="http://sohag-developer.com/";
const QString sohagDeveloperForum="http://sohag-developer.com/forum";
const QString sohagDevEditionSignature="$sohag-developer-for-postgresql-basic-1.0.0$";
const int sohagDevUpdatePeriod=10; // 10 for lightEdition and 2 for paid basic
//const QString sohagDevEditionSignature="$sohag-developer-for-postgresql-light-1.0.0$";

#endif // SOHAGGEN_H
