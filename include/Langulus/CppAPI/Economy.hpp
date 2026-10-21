///                                                                           
/// Langulus::Things                                                          
/// Copyright (c) 2013 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/Math/Vector.hpp>
#include <Langulus/Thing.hpp>
#include <Langulus/Part.hpp>


namespace Langulus::Things
{
   struct Resource;
   struct Converter;

   ///                                                                        
   ///   Abstract economy module                                              
   ///                                                                        
   struct Economy : virtual Module {
      using CTTI_Bases = Module;

      Economy() : Downcastable {this}, Module {nullptr} {}

      virtual auto GetResource (Text const&) const -> const Resource*  = 0;
      virtual auto GetConverter(Text const&) const -> const Converter* = 0;
   };

   ///                                                                        
   ///   Abstract economy part                                                
   ///                                                                        
   struct EconomyPart : virtual Part {
      using CTTI_Bases = Part;
      using CTTI_Producer = Economy;

      EconomyPart() : Downcastable {this} {}
   };
   
   struct ResourceInstance;
   struct ConverterInstance;

   ///                                                                        
   ///   Abstract resource                                                    
   ///                                                                        
   struct Resource : virtual EconomyPart {
      using CTTI_Bases = EconomyPart;
      using CTTI_Producer = Economy;

      Resource() : Downcastable {this} {}

      using Place = Math::Vec3;

      virtual int GetQuantity() const noexcept = 0;
      virtual int GetQuantityLocal(const Place&, Real radius) const noexcept = 0;

      virtual size_t GetInstanceCount() const noexcept = 0;
      virtual size_t GetInstanceCountLocal(const Place&, Real radius) const noexcept = 0;

      virtual Real GetSupply() const noexcept = 0;
      virtual Real GetSupplyLocal(const Place&, Real radius) const noexcept = 0;

      virtual Real GetDemand() const noexcept = 0;
      virtual Real GetDemandLocal(const Place&, Real radius) const noexcept = 0;

      virtual Real GetConsumption() const noexcept = 0;
      virtual Real GetConsumptionLocal(const Place&, Real radius) const noexcept = 0;

      virtual auto At(const Place&) const noexcept -> const ResourceInstance* = 0;
   };
   
   ///                                                                        
   ///   Abstract resource instance                                           
   ///                                                                        
   struct ResourceInstance : virtual EconomyPart {
      using CTTI_Bases = EconomyPart;
      using CTTI_Producer = Economy;
      
      ResourceInstance() : Downcastable {this} {}

      virtual int  GetQuantity()    const noexcept = 0;
      virtual int  GetCapacity()    const noexcept = 0;
      virtual Real GetSupply()      const noexcept = 0;
      virtual Real GetDemand()      const noexcept = 0;
      virtual Real GetConsumption() const noexcept = 0;
   };
   
   ///                                                                        
   ///   Abstract converter                                                   
   ///                                                                        
   struct Converter : virtual EconomyPart {
      using CTTI_Bases = EconomyPart;
      using CTTI_Producer = Economy;
      
      Converter() : Downcastable {this} {}

      using Place = Math::Vec3;

      virtual size_t GetInstanceCount() const noexcept = 0;
      virtual size_t GetInstanceCountLocal(const Place&, Real radius) const noexcept = 0;

      virtual auto At(const Place&) const noexcept -> const ConverterInstance* = 0;
   };

   ///                                                                        
   ///   Abstract converter instance                                          
   ///                                                                        
   struct ConverterInstance : virtual EconomyPart {
      using CTTI_Bases = EconomyPart;
      using CTTI_Producer = Economy;
      
      ConverterInstance() : Downcastable {this} {}
   };

   ///                                                                        
   ///   Abstract trader                                                      
   ///                                                                        
   struct Trader : virtual EconomyPart {
      using CTTI_Bases = EconomyPart;
      using CTTI_Producer = Economy;

      Trader() : Downcastable {this} {}
   };
}