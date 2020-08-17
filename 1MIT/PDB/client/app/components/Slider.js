import React from 'react';
import styled from 'styled-components';

import { Slider } from 'office-ui-fabric-react/lib/Slider';

const StyledSlider = styled(Slider)`


    &.ms-Slider {
    
      .ms-Slider-container {
        height: 32px;
      }
    
      .ms-Slider-value {
        text-align: right;
      }
    }

`;

export default StyledSlider;