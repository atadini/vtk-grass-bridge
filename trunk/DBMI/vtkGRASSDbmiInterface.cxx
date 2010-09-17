
/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSDbmiValue.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>
#include "vtkGRASSDbmiCatValueArray.h"


vtkCxxRevisionMacro(vtkGRASSDbmiInterface, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSDbmiInterface);

//----------------------------------------------------------------------------

vtkGRASSDbmiInterface::vtkGRASSDbmiInterface() {
    this->driver =  NULL;
    this->Connected = false;
    this->FieldNumber = 1;
    this->VectorMap = NULL;
}

//----------------------------------------------------------------------------

vtkGRASSDbmiInterface::~vtkGRASSDbmiInterface() {

    this->DisconnectDB();
    
    if(this->driver)
        G_free(this->driver);
}

//----------------------------------------------------------------------------

int vtkGRASSDbmiInterface::GetNumberOfRows()
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        char buff[1024];
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        G_snprintf(buff, 1024, "SELECT * FROM %s", Fi->table);
        dbString sql;
        db_init_string(&sql);
        db_set_string(&sql, buff);
        return db_get_table_number_of_rows(this->driver, &sql);
    }
    return -1;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::DisconnectDB()
{
    if(this->driver && this->Connected) {
        db_close_database_shutdown_driver(this->driver);
    }
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::SelectValue(int cat, const char* column, vtkGRASSDbmiValue* value)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(db_select_value(this->driver, Fi->table, Fi->key, cat, column, value->GetPointer())<0)
            return false;
    }else {
        return false;
    }
    return true;
}

bool vtkGRASSDbmiInterface::SelectCatValueArray(const char *column, const char *where, vtkGRASSDbmiCatValueArray *catval)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(db_select_CatValArray(this->driver, Fi->table, Fi->key, column, where, catval->GetPointer())<0)
            return false;
    }else {
        return false;
    }
    return true;
}



//----------------------------------------------------------------------------

void
vtkGRASSDbmiInterface::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
    os << indent << "Field number: " << this->FieldNumber << endl;
    os << indent << "Connected: " << this->Connected << endl;
    os << indent << "Registered vector map: " << (this->VectorMap?this->VectorMap->GetFullName():"none") << endl;
    if(this->VectorMap)
        this->VectorMap->PrintSelf(os, indent.GetNextIndent());
}
